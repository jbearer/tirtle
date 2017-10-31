#include <Arduino.h>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "tirtle/path.h"
#include "tirtle/robot.h"

using namespace tirtle;

/*-------------------------------------------------------------------------------
 *                               ARDUINO PINS
 --------------------------------------------------------------------------------*/
static const int enR = 10;
static const int dirR1 = 9;
static const int dirR2 = 8;
static const int dirL1 = 7;
static const int dirL2 = 6;
static const int enL = 5;

static const int L = 0;
static const int R = 1;
static const int FWD = 1;
static const int BACK = 0;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@   MOTOR SETTINGS    @@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

static const uint8_t SPEED = 100; /* Speed 0-255 (pwm value) */
static const uint8_t TURN_SPEED = 100; /*Slow speed*/

/*-------------------------------------------------------------------------------
 *                                MOTOR CONTROLS
 --------------------------------------------------------------------------------*/

/* Motor: 0 = Left, 1 = Right */
static void setDir(bool motor, bool dir)
{
    /* Choose pins based on motor */
    int dir1, dir2;
    if(motor){
        dir1 = dirL1;
        dir2 = dirL2;
    }
    else{
        dir1 = dirR1;
        dir2 = dirR2;
    }
    /* Digital write direction pins */
    if(dir){ // FWD
        digitalWrite(dir1, LOW);
        digitalWrite(dir2, HIGH);
    }
    else{
        digitalWrite(dir1, HIGH);
        digitalWrite(dir2, LOW);
    }
}

static void moveFwd()
{
    /* Set PWM values, (same speed) */
    analogWrite(enL, SPEED+2);
    analogWrite(enR, SPEED);
    setDir(L, FWD);
    setDir(R, FWD);
}

/* Dir: 0 = CCW, 1 = CW */
static void turn(bool dir)
{
    /* Slowly rotate */
    analogWrite(enL, TURN_SPEED+2);
    analogWrite(enR, TURN_SPEED);
    if(dir){ /* Clockwise */
        setDir(L, FWD);
        setDir(R, BACK);
    }
    else{ /* Counterclockwise */
        setDir(L, BACK);
        setDir(R, FWD);
    }
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@   ABSOLUTE VALUE    @@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* determines the absolute value of two coord_t types*/
static coord_t absolute(coord_t x, coord_t y)
{
    if (y <= x){
        return(x-y);
    }
    else{
        return(y-x);
    }
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@    DELTA_D    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

static coord_t delta_d(coord_t del_x, coord_t del_y)
{
  /* distance between the previous point and goal point*/
  return sqrt((del_y*del_y) + (del_x*del_x));
}

static coord_t atan2(coord_t del_y, coord_t del_x)
{
    double sdel_x = del_x;
    double sdel_y = del_y;
    return atan2(sdel_y, sdel_x) * RAD_TO_DEG;
}


/* list the possible states that tirtle can be in */
enum robot_state
{
    WAIT_FOR_IMAGE,
    MOVE_FWD,
    TURN,
    STOP
};

struct tirtle::robot_impl
{
    image                   img;
    robot_state             current_state;

    /* current_path_num to keep track of the path that the tirtle is on*/
    image::const_iterator   current_path;

    /* current_segment_num to keep track of the points that the tirtle must reach on the path*/
    path::const_iterator    current_segment;

    point                   current_loc;
    angle_t                 current_angle;


    /* define the turning direction */
    bool                    turn_direction = 0;

    /* allowable error margin between tirtle position and position of point along path*/
    static const coord_t error_margin = 1;

    /* (%) allowable percent deviation between tirtle slope and desired path slope */
    static const coord_t allowable_error = 1;

    robot_impl()
        : current_state(WAIT_FOR_IMAGE)
        , current_path(img.end())
    {}

    void update();

private:
    bool set_goal(coord_t & x, coord_t & y);
    bool set_delta(coord_t & x, coord_t & y);
};

tirtle::robot::robot()
    : impl(new robot_impl)
{
    /* Set pins and stuff */
    pinMode(enR, OUTPUT);
    pinMode(enL, OUTPUT);
    pinMode(dirL1, OUTPUT);
    pinMode(dirL2, OUTPUT);
    pinMode(dirR1, OUTPUT);
    pinMode(dirR2, OUTPUT);
    digitalWrite(enR, LOW);
    digitalWrite(enL, LOW);
}

tirtle::robot::~robot()
{
    delete impl;
}

void tirtle::robot::step()
{
    impl->update();
    switch(impl->current_state) {
    case(MOVE_FWD):
        moveFwd();
        break;
    case(TURN):
        turn(impl->turn_direction);
        break;
    case(STOP):
        exit(0);
        break;
    default:
        break;
   }
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@@   LOAD IMAGE    @@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* load image paths and store them for use */
void tirtle::robot::load_image(const image & img_)
{
    impl->img = img_;
    impl->current_path = impl->img.begin();
    impl->current_segment = impl->current_path->begin();
}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@   SET_POSITION   @@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

void tirtle::robot::set_position(const point & loc, angle_t angle)
{
    impl->current_loc = loc;
    impl->current_angle = angle;
}

bool tirtle::robot_impl::set_goal(coord_t & x, coord_t & y)
{
    if (current_path == img.end()) {
        current_state = WAIT_FOR_IMAGE;
        return false;
    }

    auto goal_point = current_segment + 1;

    if (goal_point >= current_path->end()) {
        if (++current_path < img.end()) {
            current_segment = current_path->begin();
            goal_point = current_segment + 1;
        } else {
            current_state = STOP;
            return false;
        }
    }

    x = goal_point->x;
    y = goal_point->y;
    return true;
}

bool tirtle::robot_impl::set_delta(coord_t & del_x, coord_t & del_y)
{
    coord_t x1 = current_loc.x;
    coord_t y1 = current_loc.y;

    coord_t x2, y2;
    if (!set_goal(x2, y2)) {
        return false;
    }

    /* delta x and delta y between goal and current position */
    del_x = absolute(x2, x1);
    del_y = absolute(y2, y1);
    return true;
}

void tirtle::robot_impl::update()
{
    /*-------------------------------------------------------------------------------
     * DETERMINE WHETHER TIRTLE HAS REACHED NEXT POINT ON PATH [AND UPDATE IF IT HAS]
     --------------------------------------------------------------------------------*/
    coord_t del_x, del_y;
    if (!set_delta(del_x, del_y)) {
        return;
    }

    /*if the distance between the current point and the goal point is within the margin of error*/
    while (delta_d(del_x, del_y) <= error_margin) {
        ++current_segment;
        if (!set_delta(del_x, del_y)) {
            return;
        }
    }

    /*------------------------------------------------------------------------
     * DETERMINE ANGLE AND DISTANCE BETWEEN TIRTLE AND NEXT POINT ON PATH
     ------------------------------------------------------------------------*/

    /* initialize the goal orientation */
    angle_t goal_angle = atan2(del_y, del_x);

    /*------------------------------------------------------------------------
     *    DETERMINE WHETHER TIRTLE NEEDS TO REORIENT ITSELF [IT'S OFF COURSE]
     ------------------------------------------------------------------------*/

    /* calculate asbolute error in slope */
    coord_t error = absolute(current_angle, goal_angle);

    /* determine whether tirtle is on or off course */
    if (error <= allowable_error){
        /* continue moving the tirtle in the same direction */
        current_state = MOVE_FWD;
    } else {

        if ((current_angle > goal_angle && current_angle-goal_angle > 180) ||
            (goal_angle > current_angle && goal_angle-current_angle < 180)) {
            turn_direction = 0;
        } else {
            turn_direction = 1;
        }

        current_state = TURN;
    }
}
