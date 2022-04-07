<?php
	function print_title() {
		$file = fopen('board/'.$_GET['id'], "r");
		if (!$file)
			return;
		$title = fgets($file);
		echo $title;
		fclose($file);
	}
	function print_description() {
		$file = fopen('board/'.$_GET['id'], "r");
		if (!$file)
			return;
		$title = fgets($file);
		$line ="";
		while (!feof($file))
		{
			$line .= fgets($file);
		}
		echo $line;
		fclose($file);
	}
?>
<!DOCTYPE html>
<html>
	<head>
		<title>form</title>
		<meta charset="utf-8">
	</head>
	<body>
		<!-- <form action="form.php"> -->
		<form action="form.php" method="post">
			<input type ="hidden" type="text" name="id" value="<?=$_GET['id']?>">
			<p><input type="text" name="title" placeholder="Title" value="<?php print_title(); ?>"></p>
			<p><textarea rows="8" cols="50" name="description" placeholder="Description"><?php print_description()?></textarea></p>
			<p><input type="submit"></p>
		</form>
	</body>
</html>
