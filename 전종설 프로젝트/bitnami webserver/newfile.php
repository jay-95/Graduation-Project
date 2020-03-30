<?php
 
$db_host = "localhost";
$db_user = "유저명 ";
$db_passwd = "비밀번호";
$db_name = "디비 명";
 
 
// MySQL - DB 접속.
 
$conn = mysqli_connect($db_host,$db_user,$db_passwd,$db_name);
 
if (mysqli_connect_errno()){
    
    echo "MySQL 연결 오류: " . mysqli_connect_error();
    
    exit;
    
} else {
    
    echo "DB : \"$db_name\"에 접속 성공.<br/>";
    
    $num=$_GET["num"];
    $sql = "INSERT INTO example VALUES($num)";
    $result = mysqli_query($conn, $sql);
    
    mysqli_close($conn);
    
}


출처: https://gsgdvxhx.tistory.com/2 [한빛]