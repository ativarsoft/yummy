<?php
/*
 *  yummy/trefoil.php
 *
 *  Copyright (C) 2013-2015  Mateus de Lima Oliveira
 */

header('Content-Type: image/svg+xml');

function draw_foil($angle)
{
	$r1 = 1.5 * 10;
	$p1_x = 50 + $r1 * cos(deg2rad($angle - 30));
	$p1_y = 50 + $r1 * sin(deg2rad($angle - 30));
	$p2_x = 50 + $r1 * cos(deg2rad($angle + 30));
	$p2_y = 50 + $r1 * sin(deg2rad($angle + 30));
	$r2 = 5 * 10;
	$p3_x = 50 + $r2 * cos(deg2rad($angle - 30));
	$p3_y = 50 + $r2 * sin(deg2rad($angle - 30));
	$p4_x = 50 + $r2 * cos(deg2rad($angle + 30));
	$p4_y = 50 + $r2 * sin(deg2rad($angle + 30));
	echo "<path d='M $p1_x $p1_y
	A $r1 $r1 0 0 1 $p2_x $p2_y
	L $p4_x $p4_y
	A $r2 $r2 0 0 0 $p3_x $p3_y
	z' fill='black' />";
}
?>
<svg xmlns="http://www.w3.org/2000/svg" version="1.1" height="100" width="100">
<circle cx="50" cy="50" r="10" fill="black" />
<?php
draw_foil(-30);
draw_foil(-30 - 60 * 2);
draw_foil(-30 - 60 * 4);
?>
</svg>
