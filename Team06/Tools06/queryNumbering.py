import os 

for (root, dirs, files) in os.walk('.\Team06\Tests06'):
  for name in files:
    pathname = os.path.join(root, name)
    if (pathname.endswith('_queries.txt')):
      print(pathname)
      file = open(pathname, 'r')
      queries = file.readlines()
      # file.close()

      queryNo = 1
      for i in range(0, len(queries), 5):
        comment = queries[i]
        startpos = comment.find('- ')
        if (startpos == -1):
          queries[i] = str(queryNo) + comment[startpos:]
        else:
          queries[i] = str(queryNo) + ' ' + comment[startpos:]
        queryNo += 1

      with open(pathname, 'w') as updatedfile:
        updatedfile.writelines(queries)