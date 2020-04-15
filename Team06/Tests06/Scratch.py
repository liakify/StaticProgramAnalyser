from sys import argv

# Note:
# For invalid queries:
#   test all argument input types (syn, attr, _, INT, IDENT), on all arguments
#   use Select BOOLEAN to ensure that no evaluation takes place
#   test missing arguments eg. ( , ) (one missing and all missing)
# For valid queries:
#   test white spaces (mix of tabs and spaces)

sel = "Select "
suchthat = 'such that '
pat = "Pattern"
bol = "BOOLEAN"

comment_invalid_args = " - invalid: argument type"

suchthatclauses = ['Follows ', 'Follows*', 'Parent ', 'Parent*']

designEntities = {
  "stmt": 's', 
  "read" : 'r',
  "print" : 'pn',
  "call": 'c',
  "while" : 'w',
  "if": 'ifs',
  "assign" : 'a',
  "variable": 'v',
  "constant": 'cn',
  # "procedure" : 'p',
  "prog_line": 'pl'
  }
designEntities2 = {
  # "stmt": 's', 
  # "read" : 'r',
  # "print" : 'pn',
  # "call": 'c',
  # "while" : 'w',
  # "if": 'ifs',
  # "assign" : 'a',
  "variable": 'v',
  "constant": 'cn',
  "procedure" : 'p',
  # "prog_line": 'pl'
   }

relRef = ["Follows", "Follows*", "Parent", "Parent*",
    "Modifies", "Uses", "Calls", "Calls*",
    "Next", "NextT", "Affects", "Affects*" ] 

attrNames = ["stmt#", "procName", "varName", "value"]

for de in designEntities:
  query = comment_invalid_args + '\n'
  query += de + ' ' + designEntities[de] + ';' + '\n'
  query += sel + bol + ' ' + suchthat + 'Contains (' + designEntities[de] + ', _)' + '\n'
  query += "\n"
  query += "5000"
  print(query)

for de in designEntities2:
  query = comment_invalid_args + '\n'
  query += de + ' ' + designEntities2[de] + ';' + '\n'
  query += sel + bol + ' ' + suchthat + 'Contains (_, ' + designEntities2[de] + ')' + '\n'
  query += "\n"
  query += "5000"
  print(query)


# answer = ""
# for x in range(1, 13):
#   answer += str(x) + ", "
# print(answer)

# unsorted = list(int(i) for i in argv[1].split(','))
# print(sorted(unsorted))