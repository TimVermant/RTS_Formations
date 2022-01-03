# Battle formations in RTS games
## Introduction
The usage of battle formations has been pivotal in the creation of Real-Time Strategy games since the release of some of its earliest most succesful games like Age of Empires, that to this day still remain a vastly popular game serie that uses a similar but more advanced version of the previously mentioned battle formations in its gameplay. 
Battle formations orignally came from the real world in our history where ancient empires used these to optimize the usage of their soldiers by having them grouped together and positioned in a way that was the most effective for battle.
It only makes sense that RTS games, with an overwhelming amount of game series having warfare with historical empires as an essential party of the gameplay, to want to make use of these formations to better simulate battles and the movement of multiple units.  

The way we implement this in our code is by having 1 unit be assigned as a 'leader' unit that determines both the speed and the direction of the formation unit. There are multiple parameters that determine the position of all the units relative to the leader, but the main thing will be that our leader will rotate towards a certain direction to move and all the units of that formation will then have a direction that is paralel to the leader, moving forward with a speed determined by the speed of the leader.

In this research project I will be focussing on having multiple variations on the different types of formations you can have, and having my formations move around without breaking the formation whilst being able to handle having individual units die and replacing them. 
