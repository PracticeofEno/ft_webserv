# ft_webserv
## 목적
- RFC7230 ~ 7235를 조건부로 만족하는 웹서버 구현
- 동기 Non-Blocking I/O 형태

## 환경
- linux
- 언어 : C++

## 필구 충족 사항
- 모든 매크로를 사용하고 FD_SET, FD_CLR, FD_ISSET, FD_ZERO와 같이 정의할 수 있습니다.
- 비차단적이어야 하며 클라이언트와 서버(수신 포함) 간의 모든 IO에 대해 1개의 폴링(또는 이와 동등한 것)만 사용해야 합니다.
- poll(또는 이와 동등한 것)은 읽기와 쓰기를 동시에 확인해야 합니다.
- 서버는 차단되어서는 안 되며 필요한 경우 클라이언트가 제대로 바운스되어야 합니다.
- 폴링(또는 이와 동등한 것)을 거치지 않고 읽기 작업이나 쓰기 작업을 수행해서는 안 됩니다.
- 읽기 또는 쓰기 작업 후에 errno 값을 확인하는 것은 엄격히 금지됩니다.
- 서버에 대한 요청이 영원히 중단되어서는 안 됩니다.
- 서버에 기본 오류 페이지가 없으면 기본 오류 페이지가 있어야 합니다.
- 프로그램이 누수되거나 충돌하지 않아야 합니다(메모리가 부족하더라도)
- 다른 웹 서버를 사용하면 안됨
- 클라이언트는 파일을 업로드할 수 있어야 합니다.
- HTTP 응답 상태 코드는 정확해야 합니다.
- 최소한 GET, POST 및 DELETE 메소드가 필요합니다.
- 스트레스 테스트는 서버가 어떤 비용을 치르더라도 계속 사용할 수 있어야 합니다.
- 서버는 여러 포트에서 수신할 수 있습니다(구성 파일 참조).
- FD에서 다시 읽기/수신 또는 쓰기/전송을 시도하지 않습니다.

## 필수 구현 사항
- 각 "서버"의 포트 및 호스트 선택
- server_names 설정 여부
- host:port의 첫 번째 서버는 이 host:port의 기본값이 됩니다(즉, 다른 서버에 속하지 않는 모든 요청에 응답합니다)
- 기본 오류 페이지 설정
- 클라이언트 본문 크기 제한
- 다음 규칙/구성(경로 정규 표현식을 사용하지 않음):
1. 경로에 대해 허용되는 HTTP 메서드 목록을 정의합니다.
2. HTTP 리디렉션을 정의합니다.
3. 파일을 검색해야 하는 디렉토리 또는 파일을 정의합니다(예: url /kapouet이 /tmp/www에 뿌리를 둔 경우 url /kapouet/pouic/toto/pouet은 /tmp/www/pouic/toto/pouet)
4. 디렉토리 목록 켜기 또는 끄기
5. 요청이 디렉토리인 경우 응답할 기본 파일
6. 특정 파일 확장자(예: .php)를 기반으로 CGI 실행
> CGI가 무엇인지 궁금하십니까?  
> CGI를 직접 호출하지 않기 때문에 전체 경로를 PATH_INFO로 사용합니다.  
> 청크 요청의 경우 서버가 청크를 해제해야 하며 CGI는 EOF를 본문의 끝으로 예상합니다.  
> CGI의 출력도 마찬가지입니다. content_length가 반환되지 않는 경우 CGI에서 EOF는 반환된 데이터의 끝을 의미합니다.  
> 프로그램은 요청된 파일을 첫 번째 인수로 사용하여 cgi를 호출해야 합니다.  
> cgi는 상대 경로 파일 액세스를 위한 올바른 디렉토리에서 실행되어야 합니다.  
> 서버는 하나의 CGI(php-cgi, python...)와 함께 작동해야 합니다.  
> 업로드된 파일을 수락할 수 있는 경로를 만들고 저장 위치를 구성합니다.  
- 평가 중에 작동하는 모든 기능을 테스트/시연하려면 일부 구성 파일과 기본 기본 파일을 제공해야 합니다.  

---------------

### 실행방법
```
git clone ~~
make
./webserv
```

### config 구성 예
```
http {
	server {
		server_name           blabla
		default_error_page    blabla.file
		client_body_size      8192
	  	root                  /www/            
		
		location /HAHA {
			method              GET POST DELETE
			redirection         server2.com
			path                /wwww/hoho/
			directory_listing   on | off
			default_file_to_answer_if_the_request_is_a_directory blabla.file
			cgi기본확장자 	      .cgi
			upload_directory    /www/upload
			
			return 307 blabla2 
		}

		location { } ...
	}

	server {
			server_name blabla2
         		....
  	}
}
```

------------------ 

### RFC7230 중 구현에 필요한 부분 번역
https://enocent.notion.site/RFC-7230-0dc2ce76ab414f8499ad20dc6f924d43

-------------------

### 프로젝트 구조와 역할
- 전체 구조  
![대충 구조](https://user-images.githubusercontent.com/57505385/202895546-f28e5bbd-68f4-4dd0-bc12-b422bf0aebe4.png)  
- MainServer
> init → config파일을 파싱하여 정보들로 저장.
> handleReadEvent → epoll을 사용하여 EPOLLIN (read 이벤트가 발생함) 처리
> handleWriteEvent → epoll을 사용하여 EPOLLOUT(write 가능함)이 발생시 처리
> ServerPool → 서버들을 담고있는 ServerPool보유
> ConnectionPool → 클라이언트와의 연결을 담고있는 ClientPool 보유
>
> [ ]  handleReadEvent
>> EPOLLIN 이벤트는 크게 3가지로 분류 (Server , Client, CGI)
>> Server : 웹 브라우저가 접속요청시 Server이벤트 발생. 클라이언트(Connection 클래스)를 추가함
>> Client  :  Request를 요청함 (HTTP 요청)
>> CGI : Child Process 에서 처리한 데이터를 보냄
>
> [ ]  handleWriteEvent
>> - Response의 작성이 성공적으로 끝나면 EPOLLOUT신호를 받아서 Send, 정보 Reset

- Server
> 서버의 정보들을 담고있고 Request들을 처리함
> handleRequest
>> Request 클래스의 정보들을 바탕으로 자신이 갖고있는 정보를 바탕으로 처리함  
>> 포인트 :  http://server1.com/haha 와 http://server1.com/haha/
>> 1. Location ‘/’ 의 haha파일을 요청
>> 2. 1의 요청 실패시 locathion ‘/haha’의 파일명 “”를 요청함  

- Location
> 해당 Location에 대한 정보들을 가지고있음
> 그외 잡다한 해당 로케이션의 파일경로나, 사이즈 등등을 받아오는 메소드들이 있음

- Connection
> 클라이언트와 관련된 정보들을 저장.
> Reqeust클래스와 Response클래스를 가지고있음

- Request
> 클라이언트로부터 받은 데이터를 파싱하여 Reqeust로 만듬

- Response
> 클라이언트에게 응답하기 위한 주체 (정보와 메소드를 가지고있음)

- MultiPart, UploadData
> Server에서 업로드 데이터를 편하게 처리하기위해 만든 도구들

-----------------------

### 실행 흐름
요쳥 예)
![요청헤더](https://user-images.githubusercontent.com/57505385/202898039-c518f0a5-ce37-4bc3-8c60-352376c77912.png)

1. MainServer의 ConnectionPoll에 새 클라이언트를 등록함  
![Client추가](https://user-images.githubusercontent.com/57505385/202898069-0f696f15-2701-4422-92ed-4fb81913402d.png)

2. Connection클래스에서 Reqeust클래스에게 Reqeust를 만들게 함  
![makeReqeust](https://user-images.githubusercontent.com/57505385/202898082-29f428c0-9112-45d2-b11c-a141dafb8ff4.png)

3. 만들어진 Reqeust  
![request](https://user-images.githubusercontent.com/57505385/202898099-4d1bfbca-74cc-4d13-a621-89de7d6e065f.png)

4. 만들어진 Reqeust를 바탕으로 Server에게 처리해달라고 함.  
![makeResponse](https://user-images.githubusercontent.com/57505385/202898118-db5855ea-fc38-4cc1-8a01-97b2426f2e9d.png)

5. 만들어진 Response  
![response](https://user-images.githubusercontent.com/57505385/202898128-a709522c-ed61-4cd6-840e-67668ed23c5c.png)

6. handleWriteEvent에서 response가 완성되었으면 클라이언트에게 보내고 리셋  
![2022-04-04 오후 12-36-26](https://user-images.githubusercontent.com/57505385/202898139-38524806-e165-4efe-bde7-c845d135fb46.png)

--------------------------
