//Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

/*
This function reads a line of int values into a vector function and returns that
vector.
*/
vector<int> readlineofints(int count) {
    // Input values
    vector<int> linevalues(count);
    for (int j = 0; j < count; j++) {
        int val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}



vector<long> readlineoflongs(int count) {
    // Input values
    vector<long> linevalues(count);
    for (int j = 0; j < count; j++) {
        long val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

struct point {
    double x;
    double y;
    
    //Add back default constructor
    point() = default;

    //Constructor
    point(double x_in, double y_in) :x(x_in), y(y_in) {};

    //Copy constructor
    point(const point& input) :x(input.x), y(input.y) {};
};

//Point addition function
point point_add(const point& input, double add_x, double add_y){
    double new_x = input.x + add_x;
    double new_y = input.y + add_y;
    point new_point(new_x,new_y);
    return new_point;
}

struct line {
    point start_point;
    point end_point;
    bool segment;

    //Constructor
    line(point start_in, point end_in, bool seg_in) :start_point(start_in), end_point(end_in), segment(seg_in) {};

    //Copy constructor
    line(const line& input) :start_point(input.start_point), end_point(input.end_point), segment(input.segment) {};
};

//Line addition function
line line_add(const line& input, double add_x, double add_y) {
    point new_start = point_add(input.start_point, add_x, add_y);
    point new_end = point_add(input.end_point, add_x, add_y);
    line new_line(new_start,new_end,input.segment);
    return new_line;
}

/* Find the intersection point of two lines
Each line is input as a struct of two points
The return value is a single intersection point,x1,y1
It also checks for parallel and throws and error when they are
It also doesn't verify the intersection is on the segment if one of the lines is a segment*/
point find_intersect_point(const line &first_line,const line &second_line) {
    double ax, ay, bx, by, cx, cy, dx, dy;
    ax = first_line.start_point.x;
    ay = first_line.start_point.y;
    bx = first_line.end_point.x;
    by = first_line.end_point.y;
    cx = second_line.start_point.x;
    cy = second_line.start_point.y;
    dx = second_line.end_point.x;
    dy = second_line.end_point.y;
    double x_numerator, y_numerator,x_denominator, y_denominator;

    //Check for parallel; return nothing if they are parallel
    if ((by - ay)*(dx - cx) == (bx - ax)*(dy - cy)) { throw invalid_argument("lines are parallel");}

    x_numerator = (bx - ax)*(cx*dy - dx*cy) - (dx - cx)*(ax*by - bx*ay);
    x_denominator = (by - ay)*(dx - cx) - (bx - ax)*(dy - cy);
    y_numerator = (by - ay)*(cx*dy - dx*cy) - (dy - cy)*(ax*by - bx*ay);
    y_denominator = (by - ay)*(dx - cx) - (bx - ax)*(dy - cy);

    point intersect(-x_numerator / x_denominator, -y_numerator / y_denominator);
    return intersect;
}

/*This function checks to see if the input point is on the input line segment
It assumes the point is on the line at least already (if not in the segment)*/
bool point_on_line_in_line_segment(const line &line_to_check,const  point &point_to_check) {
    bool x_in_segment = false, y_in_segment = false;
    //If x is between the line segment points (check in both directions, as we don't know whether start or end point is bigger)
    if (point_to_check.x <= line_to_check.end_point.x && point_to_check.x >= line_to_check.start_point.x) { x_in_segment = true; }
    if (point_to_check.x <= line_to_check.start_point.x && point_to_check.x >= line_to_check.end_point.x) { x_in_segment = true; }

    //Same check for y
    if (point_to_check.y <= line_to_check.end_point.y && point_to_check.y >= line_to_check.start_point.y) { y_in_segment = true; }
    if (point_to_check.y <= line_to_check.start_point.y && point_to_check.y >= line_to_check.end_point.y) { y_in_segment = true; }

    //IF both x and y were in the segment return true, otherwise return false
    if (x_in_segment && y_in_segment) {
        return true;
    }
    else {
        return false;
    }
}

/* Checks if a line intersects a segment
Note: If the line to check is also a segment, that will be checked as well
*/
bool line_intersects_segment(const line &line_to_check,const line &segment_to_check) {
    //Find intersection of both assuming they're full lines
    bool intersect;
    point intersect_point;
    try { //Try to find an intersect point
        intersect_point = find_intersect_point(line_to_check, segment_to_check);
        //Check to see if the intersect point is in the segment
        intersect = point_on_line_in_line_segment(segment_to_check, intersect_point);
    }
    catch (invalid_argument& e) { //If lines are parallel error thrown, note no intersect point
        intersect = false;
    }
       

    //If the first line isn't a segment or the above calculation returned false, return the above result
    if (line_to_check.segment == false || intersect==false) {
        return intersect;
    }
    else { //If the first line is a segment and we found an intersection, check to see if it is in the first segment too
        bool intersect2 = point_on_line_in_line_segment(line_to_check, intersect_point);
        return intersect2;
    }
}

bool ray_indicates_inside_polygon(line ray, vector<line> polygon_sides) {
    int lines_crossed = 0;
    for (line this_line : polygon_sides) {
        if (line_intersects_segment(ray, this_line)) { lines_crossed++; } //If the ray and segment intersect, increment count of lines crossed
    }

    bool inside = false;
    if (lines_crossed % 2 != 0) { inside = true; }
    return inside;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    // get test case count
    int t;
    std::cin >> t;

    //! loop over all the test cases
    for (int i = 1; i <= t; i++) {
        // Read in params
        vector<int> params = readlineofints(3);
        point impact_site(params[0], params[1]);
        long n = params[2];

        //Loop over inputs
        vector<line> sides;
        for (int j = 0; j < n; j++) {
            vector<int> this_side = readlineofints(4);
            point x = { (double) this_side[0],(double) this_side[1] };
            point y = { (double) this_side[2],(double) this_side[3] };
            line current = { x,y,true };
            sides.push_back(current);
        }

        //Throw a ray out, setting the end point to greater than any coordinate in the space
        line ray(impact_site, point(1001, impact_site.y+5), true);

        bool inside = ray_indicates_inside_polygon(ray, sides);

        //Throw a second ray out, setting the end point to greater than any coordinate in the space
        line ray2(impact_site, point(impact_site.x+5,1001), true);
        bool inside2 = ray_indicates_inside_polygon(ray2, sides);

        //Throw a third ray out, setting the end point to greater than any coordinate in the space
        line ray3(impact_site, point(1001, 1100), true);
        bool inside3 = ray_indicates_inside_polygon(ray3, sides);

        //If at least two of the three rays said we were inside, say that we are
        bool possible = false;
        int count_inside = 0;
        if (inside) { count_inside++; }
        if (inside2) { count_inside++; }
        if (inside3) { count_inside++; }
        if (count_inside >= 2) { possible = true; }


        //Print output
        if (possible) {
            std::cout << "Case #" << i << ": jackpot" << std::endl;
        }
        else {
            std::cout << "Case #" << i << ": too bad" << std::endl;
        }
    }
    return 0;
}