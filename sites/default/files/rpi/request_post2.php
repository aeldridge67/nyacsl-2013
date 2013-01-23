<?php
define( "DATABASE_SERVER", "localhost:3306" );
define( "DATABASE_USERNAME", "root" );
define( "DATABASE_PASSWORD", "root" );
define( "DATABASE_NAME", "drupal" );

//connect to the database.
$mysql = mysql_connect(DATABASE_SERVER, DATABASE_USERNAME, DATABASE_PASSWORD);

mysql_select_db( DATABASE_NAME );

// Quote variable to make safe 
function quote_smart($value)
{
 // Stripslashes
 if (get_magic_quotes_gpc()) {
 $value = stripslashes($value);
 }
 // Quote if not integer
 if (!is_numeric($value)) {
 $value = "'" . mysql_real_escape_string($value) . "'";
 }
 return $value;
}


function winpct($teamnid)
{

	$query = "SELECT * from content_type_team WHERE nid =" . $teamnid;
	$result = mysql_query($query);
	
	while ($team = mysql_fetch_object($result))
	{
 		$value = $team->field_total_wins_value / ($team->field_total_wins_value + $team->field_total_losses_value);
 	}
 	
	return $value;
}

function winpct2($wins,$losses)
{

	$value = $wins / ($wins + $losses);
	return $value;
}


/*
if( $_POST["emailaddress"] AND $_POST["username"])
{
//add the user
$Query = sprintf("INSERT INTO users VALUES ('', %s, %s)", quote_smart($_POST['username']), quote_smart($_POST['emailaddress']));

$Result = mysql_query( $Query );
}
*/

//Return a list of all team names and their node IDs
$Query = "SELECT c.nid, c.field_company_url_title, c.field_total_wins_value, c.field_total_losses_value from content_type_team AS c";
$Result = mysql_query($Query);


//Begin composition of XML
$Return = "<teams>";

//LOOP 1: Loop through every current team in the league
while ($Team = mysql_fetch_object($Result))
{

	//Initialize variables to zero
	$allWins_Opponents_Value = 0;
	$allLosses_Opponents_Value = 0;
	$allWins2_Opponents_Value = 0;
	$allLosses2_Opponents_Value = 0;

	//Return a list of certain game data for all completed games for the current team in LOOP 1
	$allGames_Query = "SELECT g.nid, g.field_game_status_nid, g.field_away_team_name_nid, g.field_home_team_name_nid, awayteam.field_total_wins_value AS awayteamwins, awayteam.field_total_losses_value AS awayteamlosses, hometeam.field_total_wins_value AS hometeamwins, hometeam.field_total_losses_value AS hometeamlosses from content_type_game AS g 
INNER JOIN content_type_team AS awayteam ON g.field_away_team_name_nid = awayteam.nid
INNER JOIN content_type_team AS hometeam ON g.field_home_team_name_nid = hometeam.nid
WHERE ((g.field_away_team_name_nid = " . $Team->nid . ") OR (g.field_home_team_name_nid = " . $Team->nid . ")) AND (g.field_game_status_nid = 25)";
	$allGames_Result = mysql_query($allGames_Query);
	
	//LOOP 2: Loop through all completed games for the current team in LOOP 1
	while ($game_Object = mysql_fetch_object($allGames_Result))
	{
		//Determine if our team was the away team for the current game in LOOP 2. If so, then we want data for our opponents -- the HOME TEAM for the current game in LOOP 2.
		if ($game_Object->field_away_team_name_nid == $Team->nid)		
		{
			$allLosses_Opponents_Value += $game_Object->hometeamlosses;
			$allWins_Opponents_Value += $game_Object->hometeamwins;
				
			//Return a list of certain game data for all completed games for the current opponent for the current game in LOOP 2
			$allGames_Query2 = "SELECT g.nid, g.field_game_status_nid, g.field_away_team_name_nid, g.field_home_team_name_nid, awayteam.field_total_wins_value AS awayteamwins, awayteam.field_total_losses_value AS awayteamlosses, hometeam.field_total_wins_value AS hometeamwins, hometeam.field_total_losses_value AS hometeamlosses from content_type_game AS g 
INNER JOIN content_type_team AS awayteam ON g.field_away_team_name_nid = awayteam.nid
INNER JOIN content_type_team AS hometeam ON g.field_home_team_name_nid = hometeam.nid
WHERE ((g.field_away_team_name_nid = " . $game_Object->field_home_team_name_nid . ") OR (g.field_home_team_name_nid = " . $game_Object->field_home_team_name_nid . ")) AND (g.field_game_status_nid = 25)";
			$allGames_Result2 = mysql_query($allGames_Query2);
			
			//LOOP 3: Loop through all completed games for the current opponent for the current game in LOOP 2
			while ($game_Object2 = mysql_fetch_object($allGames_Result2))
			{
				//Determine if our opponent was the away team for the current game in LOOP 3. If so, then we want data for their opponents -- the HOME TEAM for the current game in LOOP 3. 				//REMEMBER: We've determined that our opponent was the home team when we played against them.
				if ($game_Object2->field_away_team_name_nid == $game_Object->field_home_team_name_nid)		
				{
					$allLosses2_Opponents_Value += $game_Object2->hometeamlosses;
					$allWins2_Opponents_Value += $game_Object2->hometeamwins;
				}
				
				//Our opponent was the home team for the current game in LOOP 3. We want data for their opponents -- the AWAY TEAM for the current game in LOOP 3.
				else
				{
					$allLosses2_Opponents_Value += $game_Object2->awayteamlosses;
					$allWins2_Opponents_Value += $game_Object2->awayteamwins;
				}
			}

		}
		
		//Our team was the home team for the current game in LOOP 2. We want data for our opponents -- the AWAY TEAM for the current game in LOOP 2.
		else 
		{
			$allLosses_Opponents_Value += $game_Object->awayteamlosses;
			$allWins_Opponents_Value += $game_Object->awayteamwins;
				
			//Return a list of certain game data for all completed games for the current opponent for the current game in LOOP 2
			$allGames_Query2 = "SELECT g.nid, g.field_game_status_nid, g.field_away_team_name_nid, g.field_home_team_name_nid, awayteam.field_total_wins_value AS awayteamwins, awayteam.field_total_losses_value AS awayteamlosses, hometeam.field_total_wins_value AS hometeamwins, hometeam.field_total_losses_value AS hometeamlosses from content_type_game AS g 
INNER JOIN content_type_team AS awayteam ON g.field_away_team_name_nid = awayteam.nid
INNER JOIN content_type_team AS hometeam ON g.field_home_team_name_nid = hometeam.nid
WHERE ((g.field_away_team_name_nid = " . $game_Object->field_away_team_name_nid . ") OR (g.field_home_team_name_nid = " . $game_Object->field_away_team_name_nid . ")) AND (g.field_game_status_nid = 25)";
			$allGames_Result2 = mysql_query($allGames_Query2);
			
			//LOOP 3: Loop through all completed games for the current opponent for the current game in LOOP 2
			while ($game_Object2 = mysql_fetch_object($allGames_Result2))
			{
				//Determine if our opponent was the away team for the current game in LOOP 3. If so, then we want data for their opponents -- the HOME TEAM for the current game in LOOP 3. Remember that we've determined that our opponent was the home team when we played against them.
				if ($game_Object2->field_away_team_name_nid == $game_Object->field_away_team_name_nid)		
				{
					$allLosses2_Opponents_Value += $game_Object2->hometeamlosses;
					$allWins2_Opponents_Value += $game_Object2->hometeamwins;
				}
				
				//Our opponent was the home team for the current game in LOOP 3. We want data for their opponents -- the AWAY TEAM for the current game in LOOP 3.
				else
				{
					$allLosses2_Opponents_Value += $game_Object2->awayteamlosses;
					$allWins2_Opponents_Value += $game_Object2->awayteamwins;
				}
			}
		}
			
	}


	$pct_pre = winpct($Team->nid);
	$opp_pct_pre = winpct2($allWins_Opponents_Value, $allLosses_Opponents_Value);
	$opp_opp_pct_pre = winpct2($allWins2_Opponents_Value, $allLosses2_Opponents_Value);
	
	$rpi = round((.25 * $pct_pre) + (.5 * $opp_pct_pre) + (.25 * $opp_opp_pct_pre),3);
	$pct = round($pct_pre,3);
	$opp_pct = round($opp_pct_pre,3);
	$opp_opp_pct = round($opp_opp_pct_pre,3);
	
	$Return .= "<team><nid>".$Team->nid."</nid><teamname>".$Team->field_company_url_title."</teamname><wins>".$Team->field_total_wins_value."</wins><losses>".$Team->field_total_losses_value."</losses><pct>".$pct."</pct><opp_wins>".$allWins_Opponents_Value."</opp_wins><opp_losses>".$allLosses_Opponents_Value."</opp_losses><opp_pct>".$opp_pct."</opp_pct><opp_opp_wins>".$allWins2_Opponents_Value."</opp_opp_wins><opp_opp_losses>".$allLosses2_Opponents_Value."</opp_opp_losses><opp_opp_pct>".$opp_opp_pct."</opp_opp_pct><rpi>".$rpi."</rpi></team>";
}

$Return .= "</teams>";
mysql_free_result( $Result );
print ($Return)
?>
