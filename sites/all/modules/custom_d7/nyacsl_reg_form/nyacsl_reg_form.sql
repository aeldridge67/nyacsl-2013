SELECT
node_field_data_field_team_reference_for_stats.nid AS node_field_data_field_team_reference_for_stats_nid,
node_field_data_field_team_reference_for_stats__field_data_field_domains.delta AS node_field_data_field_team_reference_for_stats__field_data_f,
node_field_data_field_team_reference_for_stats__field_data_field_domains.language AS node_field_data_field_team_reference_for_stats__field_data_f_1,
node_field_data_field_team_reference_for_stats__field_data_field_domains.bundle AS node_field_data_field_team_reference_for_stats__field_data_f_2,
node_field_data_field_team_reference_for_stats__field_data_field_domains.field_domains_value AS node_field_data_field_team_reference_for_stats__field_data_f_3,
node_field_data_field_team_reference_for_stats__field_data_field_domains.field_domains_format AS node_field_data_field_team_reference_for_stats__field_data_f_4,
field_data_field_domains.field_domains_value AS field_data_field_domains_field_domains_value,
'node' AS field_data_field_domains_node_entity_type

FROM 
{node} node

LEFT JOIN
{field_data_field_team_reference_for_stats} field_data_field_team_reference_for_stats ON node.nid = field_data_field_team_reference_for_stats.entity_id 
AND (field_data_field_team_reference_for_stats.entity_type = 'node' AND field_data_field_team_reference_for_stats.deleted = '0')

LEFT JOIN
{node} node_field_data_field_team_reference_for_stats ON field_data_field_team_reference_for_stats.field_team_reference_for_stats_nid = node_field_data_field_team_reference_for_stats.nid

LEFT JOIN
{field_data_field_season_year} field_data_field_season_year ON node.nid = field_data_field_season_year.entity_id AND (field_data_field_season_year.entity_type = 'node' AND field_data_field_season_year.deleted = '0')

LEFT JOIN
{field_data_field_domains} node_field_data_field_team_reference_for_stats__field_data_field_domains ON node_field_data_field_team_reference_for_stats.nid = node_field_data_field_team_reference_for_stats__field_data_field_domains.entity_id AND (node_field_data_field_team_reference_for_stats__field_data_field_domains.entity_type = 'node' AND node_field_data_field_team_reference_for_stats__field_data_field_domains.deleted = '0')

LEFT JOIN
{field_data_field_domains} field_data_field_domains ON node.nid = field_data_field_domains.entity_id AND (field_data_field_domains.entity_type = 'node' AND field_data_field_domains.deleted = '0')

WHERE
(( (node_field_data_field_team_reference_for_stats.type IN  ('team')) AND (field_data_field_season_year.field_season_year_value IN  ('2012')) AND (node.nid = '2370' ) ))
ORDER BY field_data_field_domains_field_domains_value ASC