**  Hãy đọc kỹ file README này **

--Nối dây LCD --- VCC = 5V
        	NANO/UNO		MEGA
SDA pin   	   A4			 20
SCL pin  	          A5		     21




--Nối dây RFID --- VCC = 3.3V
        	NANO/UNO		MEGA
RST 	   	   9			 9
SDA   		   10		     53
SCK 		   13			 52
MOSI 		   11			 51
MISO		   12			 50



--Nối dây Sim900a --- VCC = 12V
Sim900a 	-	 NANO/UNO	-	MEGA
   RX 			    3			 3
   TX 				3			 3


--Dây khác
							NANO/UNO		MEGA     VCC
Cảm biến rung				   2			  2       5V
cảm biến hồng ngoại			   7			  7	      5V
Loa							   5			  5       5V
Chốt khóa					   6			  6       ?


** Lưu ý tất cả các module cần được nối chung GND để có thể hoạt động
