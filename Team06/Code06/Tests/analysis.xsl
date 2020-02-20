<?xml-stylesheet type="text/xsl" href="analysis.xsl"?>
<test_results>
<info>
<name>AutoTester</name><parsing_time_taken>916.000000</parsing_time_taken>
</info>
<queries>
<query>
<id ReturnTuple="1" Follows="1" SuchThat="1" CondNum="3" RelNum="1" comment="comment">2</id><querystr><![CDATA[if ifs; Select ifs such that Follows(5, ifs)]]></querystr>
<stuans>6</stuans>
<correct>6</correct>
<time_taken>30.000000</time_taken>
<passed/>
</query>
<query>
<id ReturnTuple="1" Parentstar="1" SuchThat="1" CondNum="3" RelNum="1" comment="comment">3</id><querystr><![CDATA[assign a; Select a such that Parent*(4, a)]]></querystr>
<stuans>11,5,7,8,9</stuans>
<correct>5,7,8,9,11</correct>
<time_taken>38.000000</time_taken>
<passed/>
</query>
<query>
<id ReturnTuple="1" Followsstar="1" SuchThat="1" CondNum="3" RelNum="1" comment="comment">4</id><querystr><![CDATA[call c; Select c such that Follows*(1, c)]]></querystr>
<stuans>12</stuans>
<correct>12</correct>
<time_taken>55.000000</time_taken>
<passed/>
</query>
<query>
<id ReturnTuple="1" Modifies="1" SuchThat="1" CondNum="3" RelNum="1" comment="comment">5</id><querystr><![CDATA[stmt s; Select s such that Modifies(s, "i")]]></querystr>
<stuans>11,3,4</stuans>
<correct>3,4,11,12,13,14,17</correct>
<time_taken>64.000000</time_taken>
<failed>
<missing>12,13,14,17</missing>
<additional></additional>
<summary>
<expected>7</expected>
<matched>3</matched>
<missing>4</missing>
<additional>0</additional>
</summary>
</failed>
</query>
</queries>
</test_results>
