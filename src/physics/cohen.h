#pragma once

#include "types.h"
#include "physics/line.h"
#include "physics/vertex.h"

static const u8 RIGHT  = 8;
static const u8 LEFT   = 2;
static const u8 TOP    = 4;
static const u8 BOTTOM = 1;
static const u8 INSIDE = 0;

struct AABB
{
	f32 x_min, x_max;
	f32 y_min, y_max;

	AABB(Vertex bottom_left, f32 width, f32 height) :
		x_min(bottom_left.x),
		x_max(bottom_left.x + width),
		y_min(bottom_left.y),
		y_max(bottom_left.y + height)
	{}
};

static u8 find_region(Vertex pt, AABB& dude) {
    u8 region = INSIDE;
    
    if (pt.y > dude.y_max)
        region |= TOP;
    else if (pt.y < dude.y_min)
        region |= BOTTOM;

    if (pt.x > dude.x_max)
        region |= RIGHT;
    else if (pt.x < dude.x_min)
        region |= LEFT;

    return region;
}


namespace physics {
	bool intersect_cohen(LineSegment& line, AABB& dude) {
    	u8 region[2]   = { INSIDE };
    	bool intersect = false;
    	static u64 facepalm = 0;
    
	    region[0] = find_region(line.start(), dude);
	    region[1] = find_region(line.end(), dude);

	    facepalm = 0;
	    for (;;) {
	    	if (facepalm++ > 100000000UL) return true;
	        if (!(region[0] | region[1])) {
	            intersect = true;
	            break;
	        } else if (region[0] & region[1]) {
	        	break;
	        } else {
	            f32 x, y;
	            u8 outside = region[0] ? region[0] : region[1];
	            
	            if (outside & TOP) {
	                x = line.x.p + line.x.d * (dude.y_max - line.y.p) / line.y.d;
	                y = dude.y_max;
	            } else if (outside & BOTTOM) {
	                x = line.x.p + line.x.d * (dude.y_min - line.y.p) / line.y.d;
	                y = dude.y_min;
	            } else if(outside & RIGHT) {
	                y = line.y.p + line.y.d * (dude.x_max - line.x.p) / line.x.d;
	                x = dude.x_max;
	            } else {
	                y = line.y.p + line.y.d * (dude.x_min - line.x.p) / line.x.d;
	                x = dude.x_min;
	            }

	            if (outside == region[0]) {
	            	line.start(x, y);
	            	region[0] = find_region(line.start(), dude);
	            } else {
					line.end(x, y);
	            	region[1] = find_region(line.start(), dude);
	            }
        	}
	    }

    	// TODO: Return contact point
	    return intersect;
    }
} // namespace
