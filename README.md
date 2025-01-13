# assignment1_rt

# Research Track I - First Assignment
This repository contains the assignment work for the **Research Track** course, completed by: 
**Sarvenaz Ashoori**  
**ID: 6878764**

## Introduction
This repository provides a ROS package that includes two primary nodes:
-  **User Interface node**
-  **Distance  node**
These nodes work together within the **`turtlesim`** simulation environment to create a simple, interactive system for controlling and monitoring two turtles.

## Node Overview

### 1. User Interface Node (ui_node)

This node manages user input and controls the movements of two turtles (`turtle1` and `turtle2`) within the simulation. Its main functionalities include:  
- Adding a second turtle (`turtle2`) to the simulation environment.  
- Allowing the user to:  
   - Choose which turtle to control (`turtle1` or `turtle2`).  
   - Define the linear and angular velocities for the selected turtle.  
- Sending movement commands to the chosen turtle, enabling it to move for one second. Once the movement is complete, the turtle halts, and the interface becomes ready for the next command.

### 2. Distance Node (distance_node)

This node ensures that the turtles maintain safe separation and remain within the simulation boundaries. It actively tracks and evaluates the positions of the turtles. Key functionalities include:  
- Continuously calculating the distance between `turtle1` and `turtle2` and publishing this data to a dedicated ROS topic for monitoring.  
- Automatically halting a turtle if it gets too close to the other.  
- Stopping a turtle when it approaches the simulation boundary.  

---

## Repository Layout

The root of this repository contains the package folder, which includes all the essential files and scripts to execute the assignment nodes. When cloning, ensure the repository is placed directly within the `src` directory of your ROS workspace.

### Folder and File Descriptions
- **`/msg`**: Holds custom message definitions.  
  -  distance.msg`: Defines the custom message for distance and boundary status monitoring. Includes a `float32 distance` field used for publishing distances to `distance_topic`.
     

- **`/src`**: Contains C++ source files.  
  - `ui_node.cpp`: C++ implementation of the user interface node.  
  - `distance_node.cpp`: C++ implementation of the distance monitor node.  

- **`/CMakeLists.txt`**: Defines the package build rules.  

- **`/package.xml`**: Lists the package dependencies and metadata.  

- **`/README.md`**: This documentation file.  

---

## Getting Started (Important Instructions)

### Prerequisites  
Before proceeding, confirm that **`ROS Noetic`** is installed on your system.  
If ROS is not set up yet, follow the official guide to install it:  
[ROS Noetic Installation Guide](https://wiki.ros.org/noetic/Installation/Ubuntu)  

Additionally, ensure **`Python 3`** and the **`turtlesim`** package are installed. If not, install them using the following commands:  
```bash
sudo apt-get update
sudo apt-get install ros-noetic-turtlesim
sudo apt-get install python3
```  

Once the prerequisites are installed, you can clone the repository and begin setup.  

Here’s the revised version with Python-related sections removed:

---

### Setup  

#### 1. Configure Your ROS Workspace  
Create a new workspace (or use an existing one) and navigate to its `src` directory:  
```bash
mkdir -p ~/my_new_ws/src
cd ~/my_new_ws/src
```  

#### 2. Clone the Repository  
Clone the repository into your workspace’s `src` folder:  
```bash
git clone https://github.com/Sarvenaz2000/assignment1_rt.git
```  

#### 3. Add the Workspace to the ROS Environment  
To ensure the workspace is automatically sourced in every new terminal session, add it to your `.bashrc` file:  
```bash
echo "source ~/my_new_ws/devel/setup.bash" >> ~/.bashrc
source ~/.bashrc
```  

#### 4. Build the Package  
Navigate to the root of your workspace and build the package using `catkin_make`:  
```bash
cd ~/my_new_ws
catkin_make
```  
Once the build is complete, your workspace is ready to launch the nodes.  

---

## Launching the Simulation and Nodes  

#### 1. Start the ROS Master  
Ensure the ROS Master is running before launching any nodes. Open a terminal and start `roscore`:  
```bash
roscore
```  

#### 2. Launch the Turtlesim Node  
In a new terminal, start the `Turtlesim` node to initialize the simulation environment:  
```bash
rosrun turtlesim turtlesim_node
```  
This opens the Turtlesim window, where `turtle1` and `turtle2` will appear.  

#### 3. Run User Interface and Distance Monitor Nodes  

---

#### Running the C++ Nodes  
Ensure `roscore` and `turtlesim` are running, then:  
- Start the **C++ User Interface Node**:  
```bash
rosrun assignment1_rt ui_node
```  
- Start the **C++ Distance Monitor Node**:  
```bash
rosrun assignment1_rt distance_node
```  

---

#### 4. Stop the Nodes  
To stop the nodes, press `Ctrl+C` in the terminal where they are running. This will terminate the nodes and halt the simulation.  

---

## Implementation Details  

### User Interface Node  
The `user_interface` node processes user inputs, sets turtle velocities, and publishes movement commands.  

A key feature of the **C++** implementation is its handling of duplicate turtles in the simulation. If `turtle2` already exists when the node starts, it will not attempt to respawn it. Instead, it reuses the existing `turtle2` and displays a message confirming this. This behavior ensures that restarting the node does not disrupt the simulation or cause errors.  

--- 

