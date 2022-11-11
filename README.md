# ft_webserv
- RFC7230 ~ 723x
- 동기 Non-Blocking I/O방식  
### 구현한 기능
- config 파일 (nginx와 흡사하게 만듬)
- 다중 서버 실행(config파일에 여러개 서버설정) 
- Client Body 체크 
- Location, Deirectory Listing, File Upload(MIME TYPE), CGI  
### 실행방법
```
git clone ~~
make
./webserv
```
