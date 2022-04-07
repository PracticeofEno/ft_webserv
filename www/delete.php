
<!DOCTYPE html>
<html>
	<head>
		<title>form</title>
		<meta charset="utf-8">
	</head>
	<body>
		<h1>Are you sure you want to <b>delete</b> it?</h1>
		<form action="index.php" method="post">
			<input type ="hidden" type="delete" name="delete" value="0">
			<p><input type="submit" value="No"></p>
		</form>
		<form action="form.php" method="post">
			<input type ="hidden" type="text" name="id" value="<?=$_GET['id']?>">
			<input type ="hidden" type="delete" name="delete" value="1">
			<p><input type="submit" value="Yes"></p>
		</form>
	</body>
</html>

<?php
	// var_dump($_POST['id']);
	
	// if (is_file('./board/'.$_POST['id'])) {
	// 	$path = 'board/'.$_POST['id'];
	// 	unlink(realpath($path));
	// 	$file = fopen('board/'.$_POST['id'], 'w');
	// 	fwrite($file, $_POST['title']."\n".$_POST['description']);
	// 	fclose($file);
	// 	header('Location: /index.php?id='.$_POST['id']);
	// }
	// else {

	// 	$list = scandir('./board');
	// 	$max = 0;
	// 	$i = 2;
	// 	while ($i < count($list)){
	// 		if ($max < (int)$list[$i]) {
	// 			$max = (int)$list[$i];
	// 		}
	// 		$i = $i + 1;
	// 	}
	// 	$max = $max + 1;
	// 	file_put_contents('board/'.$max, $_POST['title']."\n".$_POST['description']);
	// 	// header('Location: /index.php?id='.$max);
	// }
?>
