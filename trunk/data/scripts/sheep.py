try : 
	sheep_state
except NameError :
	sheep_state = { }
	sheep_born = { }

def sheep_onInit(curID):

	#initEntity(curID, 5, 1)

	sheep_state[curID] = 0
	sheep_born[curID] = getTime()
	
	
	loadMenu(curID,"data/UI/sheep.ui")
	setSpeed(curID,1,2)	
	setHealth(curID, 5)
	setModel(curID,"data/models/sheep.md2","data/models/Sheep.JPG")
	setUpdateInterval(curID,4*random())

def sheep_onThink(curID):
	pathTo(curID, int(getXpos(curID) + ((random() - 0.5)*2)), int(getZpos(curID) + ((random() - 0.5)*2)) )
	
	if getTime() - sheep_born[curID] > 20*3:
		killent(curID)
	
def sheep_onAttacked(curID, attackerID):
	pathTo(curID, getXpos(curID) + (getXpos(attackerID) - getXpos(curID)), getZpos(curID) + (getZpos(attackerID) - getZpos(curID)) )
	
def sheep_onDeath(curID):
	pass
	
def sheep_onSelected(curID):
	showMenu(curID)
	
def sheep_onUnselected(curID):
	hideMenu(curID)
	
def sheep_onSelectedPeasant(curID):
	setCompleted(spawnunit('peasant',getXpos(curID)+1, getZpos(curID)),100)
	killent(curID)
	
def sheep_onSelectedWoman(curID):
	setCompleted(spawnunit('woman',getXpos(curID)+1, getZpos(curID)),100)
	killent(curID)
