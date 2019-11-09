# autonomous-drone

## Project Outline

The primary objective of this project is to add some basic autonomous features to an existing drone. 
We will define our understanding of "basic autonomous features" later.
The drone we chose for our project is the Parrot AR Drone 2.0. Although it offers little customizability such as adding additional sensors if necessary, and the onboard sensors are fairly limited, it is quite affordable and allows us to focus on extracting as much information as possible from just one camera.
A core objective of ours is to obtain depth information from one camera alone using information we have on the transformation of the drone with respect to the transmitted images. 
Ideally we will be able to reconstruct a 3D map from this information, to help the drone navigate. 

## Members

| Name     | Surname | Github username | Role |
| -------- | ------- | --------------- | ---- |
| Clemence | Allard  | clemenceallard  |      |
| Yanliu   | Li      |                 |      |
| William  | Koch    | wak31415        |      |
| Joshua   | Jacob   | joshuapjacob    |      |
| Noah     | Sarfati | NoahSfi         |      |

## Structuring the Project

We will have have two core teams that each require at least two people. 

### 1. Depth Estimation and Mapping

### 2. Trajectory Planning

This team will implement the intelligent features of the drone. Given the depth information (which is continuously being updated), the drone will need to make decisions on which path to fly. This will depend on the task of the drone.

## Milestones

### 1. Initial communication

Establish first connection to drone, learn how to use the SDK to control basic flight trajectories of the drone. 
At this stage, camera data will not be evaluated.

## 2. Measure distances with a webcam

We will use a webcam to test the algorithm we write to obtain information on distances. The program should be able to detect flat surfaces and compute the distance of the camera to this surface. The surface should contain some sort of texture (not plain color).

## 3. Construct 3D map

Use previous information to construct a 3D map, containing the coordinates of points. The algorithm should also be able to determine the relevance of points, removing them if the error is to large, or if there are not enough points to describe a potential object. This will ensure that no objects are detected that do not exist. 
Figure out an efficient way to store and update this information. When the drone changes rooms for example, the dataset should be easily extendable to include the new data.

## 4. Basic drone flight

The drone should now be able to utilize the 3D-map to fly in random directions - and change direction once it comes to close to a wall.

## 5. Detecting moving objects

We want to be able to distinguish static objects from moving ones. Once a moving object is detected, we want to obtain information on its location with respect to the drone (or in the 3d-map), as well as its velocity and direction.

OPTIONAL: extrapolate movement of the object
