try : 
	ninja_state
except NameError :
	ninja_state = { }
	ninja_target = { }

	ninja_attackRadius = 10
	ninja_visionRadius = 255
	ninja_attackDamage = 10
	ninja_health = 160
	ninja_speed = 4
	

def ninja_onInit(curID):

	initEntity(curID, 9, 1)

	ninja_state[curID] = 0

	setModel(curID,"data/models/peasant.md2","data/models/peasant.JPG")
	loadMenu(curID,"data/UI/peasant.ui")


	setTeam(curID, 2) 
	setDamage(curID, ninja_attackDamage )
	setHealth(curID, ninja_health )
	setSpeed(curID, ninja_speed, ninja_speed)

def ninja_onThink(curID):


	#hunting
	if ninja_state[curID] == 0:
		ninja_target[curID] = getNearestEntity(curID, -1, 1, 1)

		if ninja_target[curID] != 0:

			if distance(ninja_target[curID], curID) <= ninja_visionRadius:
				ninja_state[curID] = 1
				stop(curID)
				if distance(ninja_target[curID], curID) > ninja_attackRadius:
					pathToEnt(curID, ninja_target[curID])

			else:
				ninja_target[curID] = getNearestEntity(curID, -1, 2, 1)
				if ninja_target[curID] != 0:
					if distance(ninja_target[curID], curID) <= ninja_visionRadius:
						ninja_state[curID] = 2
						echo("attacking a building")
		else:
			ninja_target[curID] = getNearestEntity(curID, -1, 2, 1)
			if ninja_target[curID] != 0:
				if distance(ninja_target[curID], curID) <= ninja_visionRadius:
					ninja_state[curID] = 2
					echo("attacking a building")

	# attacking a unit
	if ninja_state[curID] == 1: 
	
		if getHealth(ninja_target[curID]) <= 0:
			ninja_state[curID] = 0

		if distance(curID, ninja_target[curID]) > ninja_attackRadius:
			# move towards the target
			if getState(curID) == 2:
				stop(curID)
				pathToEnt(curID, ninja_target[curID])

		else:
			stop(curID)
			# attack			
			echo ("Ninja attacking a unit!")
			attack(curID, ninja_target[curID])
			

	# attacking a building... keep wary for units closing in
	if ninja_state[curID] == 2:

		if distance(curID, ninja_target[curID]) > ninja_attackRadius:
			# move towards the target
			pathToEnt(curID, ninja_target[curID])
		else:
			stop(curID)

			# attack
			echo ("Ninja attacking a building!")
			attack(curID, ninja_target[curID])
		
 
def ninja_onAttacked(curID, attackerID):

	if ninja_state[curID] != 1:
		if distance(curID, attackerID) < ninja_visionRadius:
			echo ("ninja returning fire!")
			ninja_target[curID] = attackerID
			ninja_state[curID] = 1

	
def ninja_onDeath(curID):
	echo("ninja died")
	
def ninja_onSelected(curID):

	pass
	
def ninja_onUnselected(curID):
	
	pass
	
