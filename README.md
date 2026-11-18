
# KFUPM KRT – VEX U Robotics

This repository contains the software developed for the **KFUPM Robotics Team (KRT)** at **King Fahd University of Petroleum and Minerals (KFUPM)** for participation in the **VEX U Robotics Competition**.

The project focuses on the development of robotic control systems, autonomous functionality, and hardware integration using **C++ and PROS**.

The repository serves as a collaborative workspace for the team's software development, testing, and continuous improvement throughout the competition.

## Technologies

- C++
- PROS (Purdue Robotics Operating System)
- VEX V5 Robotics Platform

## Theeb drivetrain calibration

Theeb has green 200 RPM motor cartridges. Each 60-tooth motor gear drives a 36-tooth wheel gear, so 216 motor degrees turn a wheel once. The signed ports and measured distance scale live in [Theeb’s profile](include/drivetrains/theeb.hpp). Another robot can use the same [distance conversion](include/drivetrains/calibration.hpp) with its own gearing and wheel travel.

Build from the repository root in a PROS terminal. The usual `pros make` and `pros upload` commands build and upload the team program. To test Theeb, choose a target below and upload its binary explicitly. This example uses Brain slot 8. Choose a free slot on your Brain.

```sh
make theeb-100cm
pros upload bin/theeb-100cm/monolith.bin --target v5 --name Theeb-100cm --slot 8 --after screen
```

| Build target | What it does |
| --- | --- |
| `theeb-50cm`, `theeb-100cm` | Press and release START on the Brain or press controller Y. After two seconds the robot drives once at 30 motor RPM. Touch the screen or press X to cancel. Restart the program to repeat. |
| `theeb-roll` | Start at a floor mark and push the robot by hand. The Brain displays signed left, right, and average travel in cm. |
| `theeb-ticks` | Turn the wheels by hand. The Brain displays each motor’s raw encoder change since startup, preserving its direction. |

Use the chosen target name in both the build command and the binary path. Measure floor travel from the same point on the chassis before and after a run. The Brain estimates travel from motor encoders. The initial 50 cm check measured 49.1–49.2 cm and supplied the correction in the profile. After correction, the 50 cm run was about 50 cm and two 100 cm runs measured about 99 and 102 cm.

## Copyright & Usage

**© 2026 KFUPM Robotics Team (KRT). All Rights Reserved.**

This project is developed for the KFUPM Robotics Team. Unauthorized reproduction, redistribution, or modification of the team's original source code is prohibited without prior permission from the respective rights holders.

Third-party libraries and dependencies remain subject to their respective licenses.
