<?php
	if ((int)$_POST['delete'] == 1) {
		$path = 'board/'.$_POST['id'];
		unlink(realpath($path));
		header('Location: /index.php');
	}
	else if (is_file('./board/'.$_POST['id'])) {
		$path = 'board/'.$_POST['id'];
		unlink(realpath($path));
		$file = fopen('board/'.$_POST['id'], 'w');
		fwrite($file, $_POST['title']."\n".$_POST['description']);
		fclose($file);
		header('Location: /index.php?id='.$_POST['id']);
	}
	else {
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
		file_put_contents('board/'.$max, $_POST['title']."\n".$_POST['description']);
		header('Location: /index.php?id='.$max);
	}
?>