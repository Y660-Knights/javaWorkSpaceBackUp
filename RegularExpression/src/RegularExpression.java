import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class RegularExpression {
	 static int Num = 1;

	public static void main(String[] args) {
		//简单认识正则表达式的概念；
		/*
		p("[a-z]{2}".matches("au"));
		p("au".matches("[a-z]{2}"));
		Pattern p = Pattern.compile("[a-z]{4}");
		Matcher m = p.matcher("arwf");
		p(m.matches());
		p("bfsafk".matches("......"));*/

		
		//简单认识 .  ? * + 
		/*
		p("a".matches("a"));
		p("a".matches("."));
		p("aa".matches("a*"));
		p("aa".matches("a?"));
		p("aaaa".matches("a+"));
		p("".matches("a?"));
		p("".matches("a*"));
		p("".matches("a+"));
		p("192.168.1.l".matches("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"));
		p("156".matches("[1-2][0-9][0-9]"));
		*/
		
		//范围
		
/*		p("a".matches("[asdf]"));
		p("a".matches("[a-z]"));
		
		//|| 's three form
		p("A".matches("[a-zA-Z]"));
		p("A".matches("[a-z]|[A-Z]"));
		p("A".matches("[a-z[A-Z]]"));
		
		//&&
		p("a".matches("[a-z&&[abt]]"));*/
		
		
		//认识 d s w D S W
		/*
		p("\n\r\t".matches("\\s{3}")); //空白字符：[ \t\n\x0B\f\r]
		p("".matches("\\S"));
		p("wkfsnk_j897534".matches("\\w+"));
		p("fjksl_jfks".matches("\\W"));
		p("sdj8953$%#^^&".matches("[a-z]{3}\\d+[#$%^^&%#]+"));
		p("\\".matches("\\\\"));
		*/

		//boundary
		/*
		p("hello sir".matches("^j.*"));
		p("hello sir".matches("^h.*"));
		p("hello sir".matches(".*ir$"));
		p("hello sir".matches(".*i$"));
		p("hello sir".matches("^h[a-z]{3}o\\b.*"));
		p("hellosir".matches("^h[a-z]{3}o\\b.*"));
		*/
		//white lines

		//p(" \n".matches("^[\\s&&[^\\n]]*\\n$"));
				
		Pattern p = Pattern.compile("Register ?\\d? ?- ?\\(?(\\w+)\\)?");
		Matcher m = p.matcher("SPI Control Register 0 - SPICR0");
		if(m.find())
			System.out.println(1);;
		
		//开头的是一个空白符但不是换行符，出现零或者多次。结尾的是换行符。
		//p(" \n".matches("^[\\s&&[^\\n]]*\\n$"));
	
		
		//匹配E-mail 地址的正则表达式练习
	/*	
		p("40716740@qq.com".matches("[\\w[.-]]+@[\\w[.-]]+\\.[\\w]+"));
	*/	
		
		
		//认识matches find lookingAt
		//其中matches 永远匹配整个字符串,find 在整个字符串中找符合的子串,lookingAt 重第一个字符开始匹配;
		//find lookingAt 都是Matcher里的方法;
		//matches在 String 和Matcher里面都有;
		/*
		Pattern p = Pattern.compile("\\d{3,5}");
		String s = "123-456-678-9";
		Matcher m = p.matcher(s);
		p(m.matches());//判断存在
		m.reset(); //重置光标?
		p(m.find());
		p(m.start() + "---" +m.end());//显示找到字串的位置
		p(m.find());
		p(m.start() + "---" +m.end());
		p(m.find());
		p(m.start() + "---" +m.end());
		p(m.find());
		p(m.find());
		
		p(m.lookingAt());
		p(m.lookingAt());
		p(m.lookingAt());
		p(m.lookingAt());
		*/
		
		//Replacement

		/*
		Pattern p = Pattern.compile("java",Pattern.CASE_INSENSITIVE);
		Matcher m = p.matcher("Java java JavA JAVa jaVa loveJaVa hateJAVa ksjdfklsfs");
		p(m.find());
		
		m.reset();
		while(m.find()){
			p(m.replaceAll("JAVA"));
		}
		m.reset();
		
		StringBuffer buf = new StringBuffer();
		int Num =0;
		while(m.find()){
			Num++;
			if(Num%2 == 0){
				m.appendReplacement(buf, "java");
			}else{
				m.appendReplacement(buf, "JAVA");
			}
		}
		m.appendTail(buf);//把没有匹配到的字符加到buf里面去
		p(buf);*/
		
/*		String s = "123.fhjsk";
		//Pattern p = Pattern.compile("(^\\w+)(\\.[a-z[A-Z]]+$)");
		Pattern p = Pattern.compile("\\.[a-z[A-Z]]+$");
		Matcher m = p.matcher(s); 
		if(m.find()){
			//m.replaceAll(".java");
			System.out.println(m.replaceAll(".java"));
		}
		*/
		//group
		/*
		Pattern p = Pattern.compile("(\\d{3,5})([a-z]{2})");
		Matcher m = p.matcher("123ae-456re-986nh-gt45-123ds");
		while(m.find()){
			p(m.group(1));
		}*/
		
		
		//quantifier
		//次数之后再加？+则有现在的意思
		//".{3,10}+[0-9]"这里一次性吞下去最多的，还能吐出来 [0,9]吞下去,最少0个最多9个
		//".{3,10}?[0-9]"这里一次性吞下去最少的，还能吐出来
		/*
		Pattern p = Pattern.compile(".{3,10}+[0-9]");
		Matcher m = p.matcher("asdf3djkn50");
		if(m.find()){
			String s = m.start() + "-" + m.end();	
			p(s);
		}else{
			p("not match!");
		}	*/
		
		//non-capturing group 不捕获组
		//".{3}"
		//"(?=e).{3}" 以e打头且字符e算一个 
		//".{3}(?=e)" 以e结尾但字符不算3个当中的一个在这里是第四个
		//上面的（）可以没有，这里只是方便看清楚而已
		
/*		Pattern p = Pattern.compile(".{3}(?=e)");
		Matcher m = p.matcher("123e52e");
		while(m.find()){
			p(m.group());
		}*/
	
	}
	
	public static void p (Object s){
		
		System.out.print(Num);
		System.out.print("  ");
		System.out.print(s);
		System.out.println();
		Num++;
	}
}
