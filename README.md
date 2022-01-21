# Battle formations in RTS games
## Introduction
The usage of battle formations has been pivotal in the creation of Real-Time Strategy games since the release of some of its earliest most succesful games like Age of Empires.  
Battle formations orignally came from the real world where ancient empires used these to optimize the usage of their soldiers by having them grouped together and positioned in a way that was the most effective for battle.</br>
It only makes sense that RTS games, with an overwhelming amount of game series having warfare with historical empires as an essential part of the gameplay, to want to make use of these formations to better simulate battles and the movement of multiple units.  </br>

In this research project our code is constructed by having 1 unit be assigned as a 'leader' unit that determines both the speed and the direction of the formation unit. There are multiple parameters that determine the position of all the units relative to the leader, but the main thing will be that our leader will rotate towards a certain direction to move and all the units of that formation will then have a direction that is parallel to the leader, moving forward with a speed determined by the speed of the leader.</br>

In this research project the focus will be on having multiple variations on the different types of formations you can have, and having my formations move around without breaking the formation whilst being able to handle having individual units die and replacing them. </br>

## Design
In order for us to define the shape of the formation, we require a few parameters that define the shape of a formation:
![image](https://user-images.githubusercontent.com/40210931/150529515-922ca158-ce71-4c3d-80d4-97d8b6f5978c.png)



There are two main classes: the **BattleUnit** class which handles the movement and display of the units and a **Formation** class. The Formation class will designate one BattleUnit as a LeaderUnit and afterwards calculate the relative desired positions of all the other units. It will then loop over all the BattleUnits in the Formation and direct them to move towards their desired position in the formation.   

</br></br>

## Result


</br></br>

## Conclusion
