"`n########################## RUNNING SYSTEM TESTS ###########################`n"

# For cmd reference:
# Autotester.exe ..\..\Tests06\_source.txt ..\..\Tests06\_queries.txt ..\..\Tests06\_out.xml
# Autotester.exe ..\..\Tests06\withSuchthat_5_source.txt ..\..\Tests06\withSuchthat_5_queries.txt ..\..\Tests06\withSuchthat_5_out.xml
# Autotester.exe ..\..\Tests06\select_attr_source.txt ..\..\Tests06\select_attr_queries.txt ..\..\Tests06\select_attr_out.xml

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
'patternIf_0',
'patternWhile_0',
'patternIf_operators', 
'patternWhile_operators',
'suchthatPatternIf_0',
'suchthatPatternWhile_0',
'calls_0',
'callsStar_0',
'calls_1',
'callsStar_1',
'callsStar_twoDiamonds',
'callsStar_threePronged',
'valid_simple\simple_procNameEqualVarName'

# STAGED TESTS
'..\select_syn',
'..\select_valid',
'..\select_invalid',
'..\patternIf_0fix',
'..\patternWhile_0fix',
'..\tuples_invalid'
'..\next_invalidArgs',
'..\nextStar_invalidArgs',
'..\invalid_doubleClause',
'..\withSuchthat_0',
'..\withSuchthat_6'

# ONLY MODIFY THE BELOW
# '..\select_attr',
# '..\tuples_0',
# '..\next_0',
# '..\nextStar_0',
# '..\suchthatPatternIf_next+Star',
# '..\suchthatPatternWhile_next+Star',
# '..\suchthatPattern_1',
# '..\with_0',
# '..\withPatternAssign_0',
# '..\withPatternIf_0',
# '..\withPatternWhile_0',
# '..\withSuchthat_1',
# '..\withSuchthat_2',
# '..\withSuchthat_3',
# '..\withSuchthat_4',
# '..\withSuchthat_5',
# '..\andPattern_0',
# '..\andPattern_invalid',
# '..\andSuchthat_0',
# '..\andSuchthat_invalid',
# '..\andWith_0',
# '..\andWith_invalid'

# '..\affects_0'
# '..\affects_1'
# '..\affects_2'
# '..\affects_3'
# '..\affects_invalidArgs'
# '..\affectsStar_0'
# '..\affectsStar_1'
# '..\affectsStar_invalidArgs'
# '..\suchthatPattern_affects+Star'
)

$list_of_no_xml_tests = @(
'invalid_simple\simple_CondAsFalse',
'invalid_simple\simple_CondAsTrue',
'invalid_simple\simple_cyclicCalls',
'invalid_simple\simple_digitWithZero,'
'invalid_simple\simple_logicExprNoBracket',
'invalid_simple\simple_logicExprNotInCond',
'invalid_simple\simple_nonDigitConstant',
'invalid_simple\simple_nonExistProcCall',
'invalid_simple\simple_printTwoVariables',
'invalid_simple\simple_printXPrintY',
'invalid_simple\simple_procSameName',
'invalid_simple\simple_selfCalls'
)

Try {
  foreach ($test in $list_of_test_files) {
    [timespan]$timing = Measure-Command { .\Team06\Code06\Debug\AutoTester.exe $regression_test_path$test$source_suffix $regression_test_path$test$query_suffix $regression_test_path$test$output_suffix *>$null }
    # Remove-Item $regression_test_path$test$output_suffix

    [XML]$output = Get-Content $regression_test_path$test$output_suffix
    
    if ($null -eq $output) {
      "[ERROR] TEST SUITE: " + $test + $output_suffix + " not found!"
      exit 1
    }

    $status = $true

    foreach ($query in $output.test_results.queries.query) {
      if ($null -eq $query.passed) {
        if ($status) {
          "[FAILED] TEST SUITE: " + $test + " [" + $(If ($timing.Seconds -ne "0") { $timing.Seconds } else {""}) + $timing.Milliseconds + " ms]"
        }
        $status = $false
        "    | Failed TC #" + $query.id.'#text' + ": " + $query.querystr."#cdata-section"
      }
    }

    if (-not $status) {
      "`nTERMINATING SYSTEM TESTS PREMATURELY...`n"
      exit 1
    }

    "[PASSED] TEST SUITE: " + $test + " [" + $(If ($timing.Seconds -ne "0") { $timing.Seconds } else {""}) + $timing.Milliseconds + " ms]"
  }
}
Catch {
  exit 1
}

foreach ($test in $list_of_no_xml_tests) {
  [timespan]$timing = Measure-Command { .\Team06\Code06\Debug\AutoTester.exe $regression_test_path$test$source_suffix $regression_test_path$test$query_suffix $regression_test_path$test$output_suffix *>$null }

  if (Test-Path $regression_test_path$test$output_suffix) {
    "[FAILED] TEST SUITE: " + $test + $output_suffix + " found when it should not be generated!"
    exit 1
  }
  "[PASSED] TEST SUITE: " + $test + " [" + $(If ($timing.Seconds -ne "0") { $timing.Seconds } else {""}) + $timing.Milliseconds + " ms]"
}

"`nALL SYSTEM TESTS PASSED`n"
