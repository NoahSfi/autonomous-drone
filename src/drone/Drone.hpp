#pragma once

class Drone {
    public:
        Drone(); 
        ~Drone();
        double[3] static get_position(); // [x,y,z]
        double[3] static get_velocity(); // [Vx, Vy, Vz] 
        
        // the following methods control the drone
        // t indicates the time in seconds that the drone should execute this
        // command
        void forward(double t);
        void right(double t);
        void left(double t);
        void backward(double t);

        // set the height in meters above the ground
        void setHeight(double z);

    private:
        double[3] position; // [x,y,z]
        double[3] velocity; // [Vx, Vy, Vz]
        float bat_charge; // battery charge of the drone
}
