sel = "Select "
suchthat = 'such that '
pat = "Pattern"

comment_invalid_args = " - invalid: argument type"

suchthatclauses = ['Follows ', 'Follows*', 'Parent ', 'Parent*']

designEntities = {"stmt": 's', 
  "read" : 'r',
 "print" : 'pn',
 "call": 'c',
  "while" : 'w',
  "if": 'ifs',
"assign" : 'a',
 "variable": 'v',
  "constant": 'cn',
  "procedure" : 'p',
   "prog_line": 'pl'
   }

relRef = ["Follows", "Follows*", "Parent", "Parent*",
    "Modifies", "Uses", "Calls", "Calls*",
    "Next", "NextT", "Affects", "Affects*" ] 

attrNames = ["stmt#", "procName", "varName", "value"]

for de in designEntities:
  # query = comment_invalid_args + '\n'
  # query += de + ' ' + designEntities[de] + ';' + '\n'
  # query += sel + designEntities[de] + ' ' + suchthat + 'Calls(' + designEntities[de] + ', _)' + '\n'
  # query += "\n"
  # query += "5000" + '\n'
  # query += comment_invalid_args + '\n'
  # query += de + ' ' + designEntities[de] + ';' + '\n'
  # query += sel + designEntities[de] + ' ' + suchthat + 'Calls(_, ' + designEntities[de] + ')' + '\n'
  # query += "\n"
  # query += "5000"

  if (de != 'variable'):
    query = comment_invalid_args + '\n'
    query += de + ' ' + designEntities[de] + '; if ifs;' + '\n'
    query += sel + designEntities[de] + ' ' + suchthat + 'pattern ifs ' + '('+ designEntities[de] + ', _, _)' + '\n'
    query += "\n"
    # query += "5000" + '\n'
    # query += comment_invalid_args + '\n'
    # query += de + ' ' + designEntities[de] + '; if ifs;' + '\n'
    # query += sel + designEntities[de] + ' ' + suchthat + 'pattern ifs ' + '(_, '+ designEntities[de] + ')' + '\n'
    # query += "\n"
    query += "5000"
    print(query)


# answer = ""
# for x in range(1, 13):
#   answer += str(x) + ", "
# print(answer)

