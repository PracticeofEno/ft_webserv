<?php
	// $list = scandir('./board');
	// var_dump($list);
	$list = scandir('./board');
	$max = 0;
	$i = 2;
	while ($i < count($list)){
		if ($max < (int)$list[$i]) {
			$max = (int)$list[$i];
		}
		$i = $i + 1;
	}
	$max = $max + 1;
	// file_put_contents('board/'.$max, $_POST['title']."\n".$_POST['description']);
	file_put_contents('board/'.$max, $_GET['title']."\n".$_GET['description']);
	// echo $CONTENT_LENGTH."a";
?>
