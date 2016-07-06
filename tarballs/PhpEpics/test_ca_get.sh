#!/usr/bin/php -q
<?PHP
dl("php_epics.so");
echo "Doing ca_get...\n";
echo ca_type("HBJ:PBOX01:IN")."\n";
$rt=ca_get("HBJ:PBOX01:IN");
echo "\nHBJ:PBOX01:IN ".$rt."\n";

#echo "Doing ca_put...\n";
#$rt=ca_put("TEST:ai1","13.3");
#echo "Doing ca_get...\n";
#$rt=ca_get("TEST:ai1");
#echo "\nTEST:ai1 ".$rt."\n";
?>
