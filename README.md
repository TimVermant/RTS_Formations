# Battle formations in RTS games

![image](https://user-images.githubusercontent.com/40210931/150549407-d178e545-9db3-4362-b7a6-aa4f8f873ffe.png)


## Introduction
The usage of battle formations has been pivotal in the creation of Real-Time Strategy games since the release of some of its earliest most succesful games like Age of Empires.  
Battle formations orignally came from the real world where ancient empires used these to optimize the usage of their soldiers by having them grouped together and positioned in a way that was the most effective for battle.</br>
It only makes sense that RTS games, with an overwhelming amount of game series having warfare with historical empires as an essential part of the gameplay, to want to make use of these formations to better simulate battles and the movement of multiple units.  </br>

In this research project our code is constructed by having 1 unit be assigned as a 'leader' unit that determines both the speed and the direction of the formation unit. There are multiple parameters that determine the position of all the units relative to the leader, but the main thing will be that our leader will rotate towards a certain direction to move and all the units of that formation will then have a direction that is parallel to the leader, moving forward with a speed determined by the speed of the leader.</br>

The focus of this project will be on researching how formations act independently, how formations process input and how individual units in a formation behave when attacking an opposing formation. </br>

## Designing the formation
In order to dynamically form and move around our formation, there are a few things we are going to need and calculate: the shape of the formation, the position of the units, the movement of the formation, the opponent selection and the combat behaviour. All these aspects will be explained in their upcoming respective section.</br>
### The shape of the formation
![image](https://user-images.githubusercontent.com/40210931/150534147-63c3ed8e-8d7d-42d7-a63e-85f685ae5329.png)
</br>
Firstly, we divide all units over a number of formations where each formation can have a different shape. A formation is made up of multiple lines that each have a number of units on them and with a set distance between eachother. </br>
The units on every line are then given an offset from eachother which will then give the formation its shape. 
Our formation is centered around a leader unit positioned in the middle of the first line, which then decides both the general direction and the speed of the formation.
</br></br>
A formation class' sole puprpose would be to store all the units, calculate the units desired position in the formation and then tell the units where to move to. An example of what a header of this class would look like:  </br>

```cpp
class BattleUnitAgent;

class Formation final
{
public:

	Formation(size_t nrOfUnits);
	~Formation();

	void Render(float dt);
	void Update(float dt);

	void CreateFormation(float trimWorldSize);
	void AddUnit(BattleUnitAgent* pUnit);

private:
	size_t m_FormationMaxSize;
	BattleUnitAgent* m_pLeaderUnit = nullptr;
	std::vector<BattleUnitAgent*> m_pUnits;
	
	// Formation math variables
	int m_UnitsPerLine;
	float m_DistanceBetweenUnitsOnLine;
	float m_VerticalDistanceBetweenUnitsOnLine;
	float m_DistanceBetweenLines;
};

```

### The positions of the units
In order to assign every unit to a formation and have it positioned accordingly, there are a few steps we need to take: </br>
&nbsp;   1. Assign the units to their closest formation  </br>
&nbsp;   2. Distribute the units over the lines in the formation using their Y-Coordinate </br>
&nbsp;   3. Assign the position of the units on the line according to their X-Coordinate </br> 

Of course we also assume that some units might die during combat, so in order for the shape of the formation to be maintained we use units from the back to replace units in the front. This is done by only adjusting the position of neighboring units.



![image](https://user-images.githubusercontent.com/40210931/150535803-76329192-96ec-4468-8dcf-9eeaa7e3eded.png)

### The movement of the formation
We don't just want to be able to create our formation, we also want it to be able to move towards a desired position whilst maintaining the formation.
We approach this momvement in two steps: First we calculate the direction the leader of a formation should move towards, we then set all other units in the formation follow in that same direction parallel to the leader. </br>
Should the formation break while moving due to potential obstacles, we can either have our units that fell out of formation temporarily move slightly faster or have our formation slow down to have those same units catch up and retain the formation.

### Opponent selection
In order for us to have our formation function as an attacking unit, we need it to know who to target. We do this by firstly finding the most nearby opponent formation and secondly apply rules to each unit to automatically determine which opposing unit it needs to attack. 
</br>
This happens with four different schemes with each having their pros and cons: 
</br>
&nbsp;   - Relative selection:  Mirror the units position and target the unit closest to that mirrored position </br>
&nbsp;   - Leader selection:    Apply a relative selection on the leader and have every other unit also target that unit </br>
&nbsp;   - Centre selection:    Target the opponent that is positioned most nearby the center of the opponent </br> 
&nbsp;   - Nearby selection:    Target the opponent that unit is closest to </br> 


![image](https://user-images.githubusercontent.com/40210931/150545137-95f376c7-a1f7-4a3f-b5c8-df8f4b6b045c.png)



### Combat behavior
We not only need to determine the opponenent selection in combat, we also determine the combat behavior of the formation, and we define five strategies for this: </br>

&nbsp;   1. Overrun: While in this behavior, the formation will continuously keep pushing the units towards the opponent units </br>
&nbsp;   2. Hold:    While in this behavior, the units will stop moving as long as the units are within range of their attacks</br>
&nbsp;   3. Retreat: While in this behavior, units will attack the opponent units and during the cooldown period between attacks continuously retreat </br> 
&nbsp;   4. Bounce:  While in this behavior, units will attack the opponent units and during the first half the cooldown period retreat but will then during the second half move &nbsp;               toward the opponent</br>
&nbsp;   5. Border: While in this behaviour, units will move outside of the opponents weapons range while the attacks are on cooldown and stand still until the cooldown has     &nbsp;                 ended</br> 




## Conclusion
To summarise, in order for the formations to work there are a few things we need:
- Form the shape of the formation
- Position the units inside of the formation
- Move the units in formation
- Have every unit select a target opponent
- Determine the combat behavior of the formation
</br>
I picked this topic because one of the first games I ever played when I was a kid was Age of Mythology which was an RTS game that came out in 2001. Although the RTS genre isn't that popular of a genre anymore, the logic behind battle formations is still quite fascinating and is surprisingly in-depth when it comes to how advanced you can have your formations behave in your game. I'm hoping that in the future this genre becomes popular again so we can see even more complex and intricate designs of new battle formations then we already have and explore how in depth one can go when creating these battle formations.  </br>




## Sources
https://sander.landofsand.com/publications/CIG08Heijden.pdf </br>
https://sandruski.github.io/RTS-Group-Movement/ </br>
