#!C:\Users\user\Anaconda3\python.exe
print("Content-Type: text/html")
print()
print('''<!doctype html>
<html>
<head>
	<title> CBNU Algorithms </title>
	<meta charset="utf-8">
	<style>
		a {
			color:black;
			text-decoration: none;
		}
		
		.active{
			color:gray;
		}
		
		.saw{
			color:red;
		}
		
		h1 {
			font-size : 45px;
			text-align : center;
		}
	</style>
</head>

<body>
	<img src = "심볼_01.jpg" width = "50"><h1><a href = "index.html">알고리즘 3팀 웹서버</a></h1>
	<ol>
		<li><a href = "1.html">팀 소개</a></li>  
		<li><a href = "2.html" class = "saw">프로젝트 소개</a></li>
		<li><a href = "3.html" class = "saw active">과목 소개</a></li>
	</ol>


	<h2><strong>프로젝트 소개</strong></h2>
	<p style "margin-top:40px"><strong>목표</strong> : <u>지정 keyword ranking 1위 하기</u>
	<br><strong>팀원</strong> : 이원재, 이종현 이용훈, 장용수, 김지안</p>



	<p>이번 <a href = "https://en.wikipedia.org/wiki/Algorithm" target = "_blank" title = "알고리즘 specification">알고리즘</a> 프로젝트는 홈페이지를 만든 후, 이것을 구글 또는 네이버에 1순위 랭킹시키는 것이 목표입니다.</p>
	<p>목표를 이루기 위하여 이번 프로젝트시 저희가 사용할 언어는 <a href = "https://en.wikipedia.org/wiki/HTML" target = "blank">HTML</a>
	, <a href = "https://en.wikipedia.org/wiki/Cascading_Style_Sheets" target = "blank">CSS</a>
	, <a href = "https://en.wikipedia.org/wiki/JavaScript_Style_Sheets" target = "blank">JSS</a> 등입니다. 홈페이지의 경우 게시판을 만드는 것을 목표로 할 것이며, 단순 홈페이지 작성을 넘어서 타인에게 효용 가치가 있는 홈페이지를 작성할 예정입니다.</p>
	<p>검색엔진에서 교수님께서 제시한 제시어에 대하여 우리 사이트의 랭킹을 높이기 위해서는
	<br><br><strong>1. 다른 팀보다 빨리 서버를 얻어 웹 사이트를 제작한다.
	<br>2. 우리 사이트의 링크를 블로그 등 여러 곳에 링킹시킨다.</strong>
	<br><br>이 두 가지를 실행해야 합니다.
	</p>
</body>
</html>''')