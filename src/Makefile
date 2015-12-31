main = main
obj = main.o Parser.o View.o Lexer.o Tokenizer.o Regex.o


$(main): $(obj)
	g++ -o $(main) $(obj)

main.o: main.cpp
	g++ -c main.cpp

Parser.o: Parser.cpp
	g++ -c Parser.cpp

View.o: View.cpp
	g++ -c View.cpp

Lexer.o: Lexer.cpp
	g++ -c Lexer.cpp

Tokenizer.o: Tokenizer.cpp
	g++ -c Tokenizer.cpp

Regex.o: regex.cpp
	g++ -c regex.cpp

clean:
	rm *.o ../dataset/*.output ../dataset/*/*.output main

cleanMainAndO:
	rm main *.o

cleanoutput:
	rm ../dataset/*.output ../dataset/*/*.output
