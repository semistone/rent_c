import java.net.*;
import java.util.*;
public class x{
    public static void main(String[] args) throws Exception{
        System.out.println("hello");
        URL hp = new URL("http://example.com:9999/j2me/"); 
        URLConnection hpCon = hp.openConnection(); 
        System.out.println("Date: " + new Date(hpCon.getDate())); 
        System.out.println("Content-Type: " + 
                        hpCon.getContentType()); 
        System.out.println("Expires: " + hpCon.getExpiration()); 
        System.out.println("Last-Modified: " + 
                        new Date(hpCon.getLastModified())); 
    }
}
