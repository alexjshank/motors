try : 
	sheep_state
except NameError :
	sheep_state = { }
	

def sheep_onInit(curID):
	sheep_state[curID] = 0

	setModel(curID,"data/models/sheep.md2","data/models/Sheep.JPG")
	setUpdateInterval(curID,4)

def sheep_onThink(curID):
	pathTo(curID, getXpos(curID) + ((random() - 0.5)*15), getZpos(curID) + ((random() - 0.5)*15) )
	
def sheep_onAttacked(curID, attackerID):
	pathTo(curID, getXpos(curID) + (getXpos(attackerID) - getXpos(curID)), getZpos(curID) + (getZpos(attackerID) - getZpos(curID)) )
	
def sheep_onDeath(curID):
	pass
	
def sheep_onSelected(curID):
	pass
	
def sheep_onUnselected(curID):
	pass