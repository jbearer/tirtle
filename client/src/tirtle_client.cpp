#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <sys/socket.h>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>

#include "tirtle/tirtle_client.h"
#include "tirtle/logging.h"
#include "tirtle/path.h"
#include "tirtle/rpc.h"

std::ostream & operator<<(std::ostream & out, const bdaddr_t & addr)
{
    char straddr[19];
    ba2str(&addr, straddr);
    out << straddr;
    return out;
}

static bool bt_address(bdaddr_t & address,
                       const std::string & dev_name,
                       const std::chrono::seconds & timeout = std::chrono::seconds(10))
{
    bool found = false;

    // get bluetooth controller id
    int dev_id = hci_get_route(NULL);
    if (dev_id == -1) {
        tirtle::log::fatal("hci_get_route: ", strerror(errno));
    }

    // open bluetooth socket and close on exit
    auto sock = std::shared_ptr<int>(new int(hci_open_dev(dev_id)), [](auto p) {
        if (close(*p) == -1) {
            tirtle::log::perror("error closing bluetooth socket ", *p);
        }
        delete p;
    });
    if (*sock == -1) {
        tirtle::log::fatal("hci_open_dev");
    }

    tirtle::log::debug("scanning controller ", dev_id, " at socket ", *sock, " for bluetooth devices");
    const size_t MAX_DEVS = 255;
    inquiry_info *devs = new inquiry_info[MAX_DEVS];
    int bt_timeout = static_cast<int>(static_cast<double>(timeout.count()) / 1.28);
    int ndevs = hci_inquiry(dev_id, bt_timeout, MAX_DEVS, NULL, &devs, IREQ_CACHE_FLUSH);
    if (ndevs == -1) {
        tirtle::log::fatal("hci_inquiry");
    }

    tirtle::log::debug("detected ", ndevs, " bluetooth devices");
    tirtle::log::debug("searching for device with name \"", dev_name, "\"");
    for (auto dev = devs; dev < devs + ndevs; ++dev) {
        std::string name(148, '\0');
        if (hci_read_remote_name(*sock, &dev->bdaddr, name.size() - 1, &name[0], 0) == -1) {
            tirtle::log::perror("error reading device name from address ", dev->bdaddr, " skipping");
            continue;
        }

        tirtle::log::debug("discovered device ", name, " at ", dev->bdaddr);

        if (strcmp(name.c_str(), dev_name.c_str()) == 0) {
            address = dev->bdaddr;
            found = true;
            break;
        }
    }

    delete[] devs;

    return found;
}

static int bt_socket(const bdaddr_t & dest)
{
    int fd = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // point address at server
    struct sockaddr_rc addr;
    bzero(&addr, sizeof(addr));
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    addr.rc_bdaddr = dest;

    tirtle::log::debug("connecting to bluetooth device ", dest);
    int status = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
    if (status == -1) {
        tirtle::log::pfatal("connect");
    } else {
        tirtle::log::debug("successfully connected to ", dest);
    }

    return fd;
}

template<class T>
static void bt_send(int socket, T data)
{
    tirtle::log::debug("sending ", sizeof(data), " bytes to fd ", socket, ": ", data);

    size_t nbytes = 0;
    while (nbytes < sizeof(data)) {
        ssize_t status = write(socket, &data, sizeof(data));
        if (status == -1) {
            tirtle::log::pfatal("write");
        }
        tirtle::log::debug("sent ", status, "/", sizeof(data), " bytes to fd ", socket);
        nbytes += status;
    }
}

template<>
void bt_send<point_t>(int socket, point_t loc)
{
    bt_send(socket, loc.x);
    bt_send(socket, loc.y);
}

template<>
void bt_send<path_t>(int socket, path_t path)
{
    bt_send(socket, path.length);
    for (length_t p = 0; p < path.length; ++p) {
        bt_send(socket, path.points[p]);
    }
}

struct tirtle_client_impl
    : tirtle::tirtle_client
{
    tirtle_client_impl(const std::string & dev)
        : image_loaded(false)
    {
        bdaddr_t dest;
        if (!bt_address(dest, dev)) {
            tirtle::log::fatal("unable to find bluetooth device with name \"", dev, "\"");
        }
        socket = bt_socket(dest);
    }

    void load_image(const std::vector<path_t> & paths) override
    {
        if (image_loaded) {
            tirtle::log::fatal("duplicate image load");
        }

        tirtle::log::info("loading image with ", paths.size(), " paths");

        bt_send(socket, LOAD_IMAGE);
        bt_send(socket, (length_t)paths.size());
        for (const auto & path : paths) {
            bt_send(socket, path);
        }
    }

    void set_position(point_t loc, angle_t angle) override
    {
        assert(angle < 360);
        tirtle::log::debug("setting position(loc=", loc, ",angle=", angle, ")");
        bt_send(socket, SET_POSITION);
        bt_send(socket, loc);
        bt_send(socket, angle);
    }

    ~tirtle_client_impl()
    {
        if (close(socket) == -1) {
            tirtle::log::perror("error closing socket ", socket);
        }
    }

private:
    bool    image_loaded;
    int     socket;
};

std::unique_ptr<tirtle::tirtle_client> tirtle::connect_tirtle(const std::string & dev)
{
    return std::make_unique<tirtle_client_impl>(dev);
}
