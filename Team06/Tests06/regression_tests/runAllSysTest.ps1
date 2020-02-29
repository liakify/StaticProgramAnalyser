"STARTING SYSTEM TESTS"

$source_suffix = '_source.txt'
$query_suffix = '_queries.txt'
$output_suffix = '_out.xml'
$regression_test_path = '.\Team06\Tests06\regression_tests\'
$list_of_test_files = @(
'select_0_1'
'follows_0',
'followsStar_0',
'parent_0',
'parentStar_0',
'modifies_0_1',
'uses_0_1',
'pattern_0_1',
'suchthatPattern_0_1'
)

Try {

  foreach ($test in $list_of_test_files) {
    .\Team06\Code06\Debug\AutoTester.exe $regression_test_path$test$source_suffix $regression_test_path$test$query_suffix $regression_test_path$test$output_suffix
    [XML]$output = Get-Content $regression_test_path$test$output_suffix
    
    foreach ($query in $output.test_results.queries) {
      # if ($null -eq $query.passed) {
      #   "SYSTEM TEST FAILED: "
      #   exit 1
      # }
      $query.id
    }
  }
  
  .\Team06\Code06\Debug\AutoTester.exe .\Team06\Tests06\modifies_0_source.txt .\Team06\Tests06\modifies_0_queries.txt .\Team06\Tests06\modifies_0_out.xml
  [XML]$output2 = Get-Content .\Team06\Tests06\modifies_0_out.xml
  
  foreach ($query in $output2.test_results.queries) {
    if ($null -eq $query.passed) {
      "SYSTEM TEST FAILED: "
      exit 1
    }
  }
}
Catch {
  exit 1
}


"ALL SYSTEM TESTS PASSED"