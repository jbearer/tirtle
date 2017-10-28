#include "path.h"
#include "motor_control.h"

static path_t *paths;

/* load image paths and store them for use */
void load_image(const path_t *path_, length_t num_paths)
{
  paths = path_;
  
}

/* current_path_num to keep track of the path that the tirtle is on*/
int current_path_num = 0;

/* current_segment_num to keep track of the points that the tirtle must reach on the path*/
int current_segment_num = 0;

/* allowable error margin between tirtle position and position of point along path*/
coord_t error_margin = 1; 

/* allowable percent deviation between tirtle slope and desired path slope */
float allowable_percent_err = 1;

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@   ABSOLUTE VALUE    @@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

/* determines the absolute value of two coord_t types*/
 coord_t absolute(coord_t x, coord_t y){
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

coord_t delta_d(coord_t del_x, coord_t del_y){

  /* distance between the previous point and goal point*/
  return sqrt((del_y)^2 + ((del_x)^2));

}

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * @@@@@@@@@@@@@@@@@@@@@@@@   SET_POSITION   @@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

void set_position(point_t location_, angle_t angle_){

  point_t current_point = location_; /* define the current point of the tirtle*/

  /* initialize the starting coordinates of the tirtle's path*/
  coord_t x0;
  coord_t y0;

  /* Define the very first point in the turtle's path*/
  if (current_segment_num == 0){
    x0 = current_point.x;
    y0 = current_point.y;
  }
  
  /*-------------------------------------------------------------------------------
   * DETERMINE WHETHER TIRTLE HAS REACHED NEXT POINT ON PATH [AND UPDATE IF IT HAS]
   --------------------------------------------------------------------------------*/
   
  point_t goal_point = paths[current_path_num].points[current_segment_num]; /* the location of the next point that tirtle needs to get to */

   /* current x and y coordinate values of the tirtle*/
  coord_t x1 = current_point.x;
  coord_t y1 = current_point.y;

  /* x and y coordinates of the next point on tirtle's path*/
  coord_t x2 = goal_point.x;
  coord_t y2 = goal_point.y;

  /*delta x and delta y between goal and previous coordinate*/
  coord_t del_x = absolute(x2,x1);
  coord_t del_y = absolute(y2,y1);

  coord_t del_dist = del_d(del_x, del_y); /* calculate distance between current and goal point*/

  /*if the distance between the current point andthe goal point is within the margin of error*/
  if (del_dist <= error_margin){

    /* update the previous point in tirtle's path*/
    x0 = x1;
    y0 = y1;

    if (current_segment_num < paths[current_path_num].length - 1){
      /* update the goal point */
      current_segment_num = current_segment_num + 1;
      goal_point = paths[current_path_num].points[current_segment_num];
  
      /*update x and y coordinates of the next point on tirtle's path*/
      coord_t x2 = goal_point.x;
      coord_t y2 = goal_point.y;
      
      /*update delta x and delta y between goal and current coordinate*/
      del_x = absolute(x2,x1);
      del_y = absolute(y2,y1);
  
      del_dist = del_d(del_x, del_y); /* calculate distance between current and new goal point*/
    }
    else{
      /* send some sort of signal to indicate that it has finished a path */
    }
      
    }
  
  /*------------------------------------------------------------------------
   * DETERMINE ANGLE AND DISTANCE BETWEEN ROBOT AND NEXT POINT ON PATH
   ------------------------------------------------------------------------*/
   
  angle_t current_angle = angle_; /* the current orientation angle of tirtle*/

  /* initialize the goal orientation */
  angle_t goal_angle;

  /* define angle that tirtle needs to travel to reach goal point*/
  if (del_y > 0 && del_x > 0) { /* quadrant 1*/
    goal_angle = atan(del_y/del_x);
  }
  else if (del_y > 0 && del_x < 0) { /* quadrant 2*/
    goal_angle = atan(del_y/del_x) + 90;
  }
  else if (del_y <0 && del_x < 0) { /* quadrant 3*/
    goal_angle = atan(del_y/del_x) + 180;
  }
  else if (del_y<0 && del_x>0) { /* quadrant 4*/
    goal_angle = atan(del_y/del_x) + 270;
  }
  else if (del_y == 0){ /* move horizontally*/
    if (del_x < 0){
      goal_angle = 180;
    }
    else {
      goal_angle = 0;
    }
  }
  else{ /* move vertically*/
    if (del_y < 0){
      goal_angle = 270;
    }
    else{
      goal_angle = 90;
    }
  }  
  
  /*------------------------------------------------------------------------
   *    DETERMINE WHETHER TIRTLE NEEDS TO REORIENT ITSELF [IT'S OFF COURSE]
   ------------------------------------------------------------------------*/

   coord_t current_slope = del_y/del_x; /* current slope that tirtle is traveling at*/
   coord_t desired_slope = absolute(y2,y0)/absolute(x2,x0); /* desired slope between previous point and next point on path*/

   /* calculate percent error in slope */
   coord_t percent_err = absolute(current_slope,desired_slope)/desired_slope;

   /* determine whether tirtle is on or off course */
   if (percent_err*100 <= allowable_percent_err){
    /* continue doing whatever it needs to be doing */
   }
   else{
    /*ROTATE THE TIRTLE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
   }

   


  
}

