"STARTING SYSTEM TESTS"

# For cmd reference:
# Autotester.exe ..\..\Tests06\_source.txt ..\..\Tests06\_queries.txt ..\..\Tests06\_out.xml
# Autotester.exe ..\..\Tests06\patternIf_operators_source.txt ..\..\Tests06\patternIf_operators_queries.txt ..\..\Tests06\patternIf_operators_out.xml

$source_suffix = '_source.txt'
$query_suffix = '_queries.txt'
$output_suffix = '_out.xml'
$regression_test_path = '.\Team06\Tests06\regression_tests\'

$list_of_test_files = @(
  # SAMPLE FAILED TEST CASE (to verify that this works)
  # '..\modifies_0',

# DO NOT MODIFY THE FOLLOWING IN ANY COMMITS
'select_0_1',
'follows_0',
'followsStar_0',
'parent_0',
'parentStar_0',
'patternAssign_0',
'patternAssign_1',
'suchthatPatternAssign_0',
'uses_0',
'modifies_0',

# ONLY MODIFY THE BELOW
# '..\patternIf_0',
# '..\patternWhile_0',
'..\patternIf_operators', 
'..\patternWhile_operators'
# '..\select_0'
# '..\calls_0',
# '..\calls_1'
# '..\callsStar_0',
# '..\callsStar_1'
# '..\callsStar_twoDiamonds',
# '..\callsStar_threePronged',
# '..\invalid_simple\simple_CondAsFalse',
# '..\invalid_simple\simple_CondAsTrue'
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
    # Remove-Item $regression_test_path$test$output_suffix
    
    [XML]$output = Get-Content $regression_test_path$test$output_suffix
    
    if ($null -eq $output) {
      "SYSTEM TEST FAILED in: " + $test
      $test + $output_suffix + " not found. "
      exit 1
    }

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