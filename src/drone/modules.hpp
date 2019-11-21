#pragma once

// given an array of three points (that uniquely determine the equation of a 
// plane), compute the distance of the drone to this wall. The coordinates of
// the drone are given as [x,y,z] and wall contains three points [a,b,c], 
// each of which are an array of size three corresponding to the coordinates
float distance(int[3] pos, int*[3] wall);
