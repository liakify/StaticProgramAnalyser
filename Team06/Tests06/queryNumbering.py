import os

for (root, dirs, files) in os.walk('.'):
  for name in files:
    pathname = os.path.join(root, name)
    if (pathname.endswith('_queries.txt') and not pathname.startswith('.\given')):
      print(pathname)
      file = open(pathname, 'r')
      queries = file.readlines()
      # file.close()

      queryNo = 1
      for i in range(0, len(queries), 5):
        comment = queries[i]
        startpos = comment.find(' - ')
        queries[i] = str(queryNo) + comment[startpos:]
        queryNo += 1

      with open(pathname, 'w') as updatedfile:
        updatedfile.writelines(queries)