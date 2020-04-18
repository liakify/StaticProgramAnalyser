import random
import sys

# parameters
sys.setrecursionlimit(50000)
lines = 600
maxStmtInStmtList = 10
initialNestingLvl = 1
maxNestingLvl = 6
casePrefix = "performance_"
sourceSuffix = "_source.txt"
fullname = casePrefix + str(lines) + sourceSuffix

# simple
varNames = ['bunch', 'audience', 'flock', 'team', 'group', 'family', 'band', 'village']
operators = [' + ', ' - ', ' * ', ' % ', ' / ']

def getVar():
    return random.choice(varNames)
def getOperator():
    return random.choice(operators)

def getAssignment(nestinglvl):
    global lines
    output = "\n"
    for i in range(0, nestinglvl):
        output += "\t"
    output += getVar() + ' = ' + getVar()
    for i in range(1, int(random.random() * 5)):
        output+= getOperator() + getVar()
    output += ";"
    lines -= 1
    return output

def getStmtList(nestinglvl):
    stmtlist = ""
    if lines > 0:
        for i in range(1, int(random.random() * maxStmtInStmtList) + 2):
            stmt = getStmt()
            # print(stmt)
            stmtlist += stmt[0](nestinglvl)
    else:
        stmtlist += getAssignment(nestinglvl)
    return stmtlist

def getIf(nestinglvl):
    global lines
    global maxNestingLvl
    if (nestinglvl == maxNestingLvl):
        return getAssignment(nestinglvl)
    
    output = "\n"
    for i in range(0, nestinglvl):
        output += "\t"
    output += "if (c1 == c2) then {"
    output += getStmtList(nestinglvl + 1)
    output += "     } else {"
    output += getStmtList(nestinglvl + 1)
    output += "}"
    lines -= 1
    return output

def getWhile(nestinglvl):
    global lines
    global maxNestingLvl
    if (nestinglvl == maxNestingLvl):
        return getAssignment(nestinglvl)
    
    output = "\n"
    for i in range(0, nestinglvl):
        output += "\t"
    output += "while(c1 == c2) {"
    output += getStmtList(nestinglvl + 1)
    output += "}"
    lines -= 1
    return output

def getStmt():
    return random.choices([getAssignment, getIf, getWhile], weights=[5, 1, 1])

with open(fullname, 'w') as file:
    output = "procedure Long {"
    while (lines > 0):
        output += getStmtList(initialNestingLvl)
    output += "}"
    file.write(output)
    print("works")