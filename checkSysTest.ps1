"`n########################## RUNNING SYSTEM TESTS ###########################`n"

# For cmd reference:
# Autotester.exe ..\..\Tests06\_source.txt ..\..\Tests06\_queries.txt ..\..\Tests06\_out.xml
# Autotester.exe ..\..\Tests06\withSuchthat_5_source.txt ..\..\Tests06\withSuchthat_5_queries.txt ..\..\Tests06\withSuchthat_5_out.xml
# Autotester.exe ..\..\Tests06\select_attr_source.txt ..\..\Tests06\select_attr_queries.txt ..\..\Tests06\select_attr_out.xml

$debug_exe_path = '.\Team06\Code06\Debug\AutoTester.exe'
$release_exe_path = '.\Team06\Code06\Release\AutoTester.exe'
$exe_path = $release_exe_path

$source_suffix = '_source.txt'
$query_suffix = '_queries.txt'
$output_suffix = '_out.xml'
$regression_test_path = '.\Team06\Tests06\regression_tests\'

$list_of_test_files = @(
  
  '..\performance600_single'
  '..\performance600_chainedNext'

  # TEST BELOW CURRENTLY FAIL / TIMEOUT
  # '..\performance600_chainedNextStar'
  # '..\performance600_chainedAffects'
  # '..\performance600_chainedAffectsStar'
  # '..\performance600_nextStar+AffectsStar+Tuple'
  )

$list_of_no_xml_tests = @(
)

# Determine which build type of the AutoTester executable to test
$has_release = Test-Path $exe_path

if (-not $has_release) {
  $exe_path = $debug_exe_path
  $has_debug = Test-Path $exe_path

  if (-not $has_debug) {
    "[ERROR] No AutoTester executable of any build type found!"
    "`nTERMINATING SYSTEM TESTS DUE TO BUILD OUTPUT ERRORS...`n"
    exit 127
  }

  # Warn user the debug AutoTester is being system-tested
  "##      ##    ###    ########  ##    ## #### ##    ##  ######  "
  "##  ##  ##   ## ##   ##     ## ###   ##  ##  ###   ## ##    ## "
  "##  ##  ##  ##   ##  ##     ## ####  ##  ##  ####  ## ##       "
  "##  ##  ## ##     ## ########  ## ## ##  ##  ## ## ## ##   ####"
  "##  ##  ## ######### ##   ##   ##  ####  ##  ##  #### ##    ## "
  "##  ##  ## ##     ## ##    ##  ##   ###  ##  ##   ### ##    ## "
  " ###  ###  ##     ## ##     ## ##    ## #### ##    ##  ######  "
  
  "`n[WARNING] Debug version of AutoTester.exe is being system-tested instead of the release version!"
  "To remove this warning, configure Visual Studio to build the release version of AutoTester!`n"
}

Try {
  foreach ($test in $list_of_test_files) {
    $has_inputs = (Test-Path $regression_test_path$test$source_suffix) -and (Test-Path $regression_test_path$test$query_suffix)

    if (-not $has_inputs) {
      "[ERROR] TEST SUITE: " + $test + $source_suffix + " OR " + $test + $query_suffix + " not found!"
      "`nTERMINATING SYSTEM TESTS DUE TO MISSING TEST INPUTS...`n"
      exit 1
    }

    $test_command = "$exe_path $regression_test_path$test$source_suffix $regression_test_path$test$query_suffix $regression_test_path$test$output_suffix | out-null"
    [timespan]$timing = Measure-Command { Invoke-Expression $test_command }
    # Remove-Item $regression_test_path$test$output_suffix

    $has_output = Test-Path $regression_test_path$test$output_suffix

    if (-not $has_output) {
      "[ERROR] TEST SUITE: " + $test + $output_suffix + " not found!"
      "`nTERMINATING SYSTEM TESTS DUE TO MISSING TEST OUTPUT...`n"
      exit 1
    }

    [XML]$output = Get-Content $regression_test_path$test$output_suffix
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
  "[ERROR] TEST SUITE: " + $test + " resulted in an unknown error during execution!"
  "`nTERMINATING SYSTEM TESTS DUE TO MISSING TEST INPUTS...`n"
  exit 1
}

foreach ($test in $list_of_no_xml_tests) {
  $has_inputs = (Test-Path $regression_test_path$test$source_suffix) -and (Test-Path $regression_test_path$test$query_suffix)

  if (-not $has_inputs) {
    "[ERROR] TEST SUITE: " + $test + $source_suffix + " OR " + $test + $query_suffix + " not found!"
    "`nTERMINATING SYSTEM TESTS DUE TO MISSING TEST INPUTS...`n"
    exit 1
  }
  
  $test_command = "$exe_path $regression_test_path$test$source_suffix $regression_test_path$test$query_suffix $regression_test_path$test$output_suffix | out-null"
  [timespan]$timing = Measure-Command { Invoke-Expression $test_command }

  if (Test-Path $regression_test_path$test$output_suffix) {
    "[FAILED] TEST SUITE: " + $test + $output_suffix + " found when it should not be generated!"
    "`nTERMINATING SYSTEM TESTS PREMATURELY...`n"
    exit 1
  }
  "[PASSED] TEST SUITE: " + $test + " [" + $(If ($timing.Seconds -ne "0") { $timing.Seconds } else {""}) + $timing.Milliseconds + " ms]"
}

"`nALL SYSTEM TESTS PASSED`n"
