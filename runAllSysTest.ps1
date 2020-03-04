"STARTING SYSTEM TESTS"

$source_suffix = '_source.txt'
$query_suffix = '_queries.txt'
$output_suffix = '_out.xml'
$regression_test_path = '.\Team06\Tests06\regression_tests\'
$list_of_test_files = @(
  # SAMPLE FAILED TEST CASE (to verify that this works)
  # '..\modifies_0',
'select_0_1',
'follows_0',
'followsStar_0',
'parent_0',
'parentStar_0',
'modifies_0_1',
'uses_0_1',
'pattern_0',
'pattern_1',
'suchthatPattern_0_1'
# '..\invalid_simple\simple_CondAsFalse',
# '..\invalid_simple\simple_CondAsTrue',
# '..\invalid_simple\simple_cyclicCalls',
# '..\invalid_simple\simple_digitWithZero',
# '..\invalid_simple\simple_logicExprNoBracket',
# '..\invalid_simple\simple_logicExprNotInCond',
# '..\invalid_simple\simple_nonDigitConstant',
# '..\invalid_simple\simple_nonExistProcCall',
# '..\invalid_simple\simple_printTwoVariables',
# '..\invalid_simple\simple_printXPrintY',
# '..\invalid_simple\simple_procSameName',
# '..\invalid_simple\simple_varSameName',
# '..\invalid_simple\simple_selfCalls',
# '..\valid_simple\simple_procNameEqualVarName'
)

Try {
  foreach ($test in $list_of_test_files) {
    .\Team06\Code06\Debug\AutoTester.exe $regression_test_path$test$source_suffix $regression_test_path$test$query_suffix $regression_test_path$test$output_suffix *>$null
    [XML]$output = Get-Content $regression_test_path$test$output_suffix
    
    foreach ($query in $output.test_results.queries.query) {
      if ($null -eq $query.passed) {
        "SYSTEM TEST FAILED in: " + $test
        exit 1
      }
    }
    'TEST CASES in ' + $test + ' PASSED'
  }
}
Catch {
  exit 1
}

"ALL SYSTEM TESTS PASSED"