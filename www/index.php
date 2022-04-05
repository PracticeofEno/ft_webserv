<!DOCTYPE html>
<html lang = "ko">
	<head>
		<meta charset="utf-8">
		<!-- <title>board</title> -->
		<title>board</title>
	</head>
	<body>
		<h1>게시판</h1>
		<ol>
			<?php
				$list = scandir('./board');
				$i = 2;
				while ($i < count($list)){
					$file = fopen("board/$list[$i]", "r");
					$title = fgets($file);
					echo "<li><a href=\"index.php?id=$list[$i]\">$title</a></li>\n";
					fclose($file);
					$i = $i + 1;
				}
			?>
		</ol>
			<?php
				$file = fopen('board/'.$_GET['id'], "r");
				if (!$file)
					return;
				echo "<h2>";
				$title = fgets($file);
				echo $title;
				echo "</h2>";
				// echo nl2br("\n");
				while (!feof($file))
				{
					$line .= fgets($file);
				}
				echo nl2br($line);
				fclose($file);
			?>
	</body>
</html>