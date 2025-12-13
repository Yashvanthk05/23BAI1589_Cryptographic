import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class main {
    private static final char[] ENGLISH_MONO_FREQ="etaoinshrdlcumwfgypbvkjxqz".toCharArray();
    private static final String[] COMMON_BIGRAMS={"th","he","in","er","an","re","nd","on","en","at","ou","ed","ha","to","or","it","is","hi","es","ng"};
    private static final String[] COMMON_TRIGRAMS={"the","and","ing","her","hat","his","tha","ere","for","ent","ion","ter","was","you","ith","ver","all","wit","thi","tio"};
    public static void main(String[] args) throws IOException {
        String text = readFile(args[0]).toLowerCase();
        String cleanText = text.replaceAll("[^a-z ]", "");
        Map<Character, Character> monoMap=getMonoMapping(cleanText);
        Map<Character, Character> bigramMap=getBigramMapping(cleanText);
        Map<Character, Character> trigramMap=getTrigramMapping(cleanText);
        Map<Character, Character> combinedMap = new HashMap<>();
        combinedMap.putAll(monoMap);
        combinedMap.putAll(bigramMap);
        combinedMap.putAll(trigramMap);
        System.out.println("Monogram Decryption:");
        System.out.println(applyMapping(cleanText, monoMap));
        System.out.println("\nBigram Decryption:");
        System.out.println(applyMapping(cleanText, bigramMap));
        System.out.println("\nTrigram Decryption");
        System.out.println(applyMapping(cleanText, trigramMap));
        System.out.println("\nCombined Decryption");
        System.out.println(applyMapping(cleanText, combinedMap));
        System.out.println("\nMonogram Mapping: " + monoMap);
        System.out.println("Combined Mapping: " + combinedMap);
    }

    private static Map<Character, Character> getMonoMapping(String text){
        Map<Character, Integer> freq=new HashMap<>();
        for(char c : text.toCharArray()) if(c!=' ') freq.put(c,freq.getOrDefault(c,0)+1);
        List<Character> sorted=freq.entrySet().stream().sorted((a,b) -> b.getValue()-a.getValue()).map(Map.Entry::getKey).collect(Collectors.toList());
        Map<Character, Character> map=new HashMap<>();
        for(int i=0;i<sorted.size() && i<ENGLISH_MONO_FREQ.length;i++)
            map.put(sorted.get(i), ENGLISH_MONO_FREQ[i]);
        return map;
    }

    private static Map<Character, Character> getBigramMapping(String text){
        String compact=text.replaceAll(" ","");
        Map<String,Integer> freq=new HashMap<>();
        for(int i=0;i<compact.length()-1;i++){
            String bi=compact.substring(i,i+2);
            freq.put(bi,freq.getOrDefault(bi,0)+1);
        }
        List<String> sorted=freq.entrySet().stream().sorted((a,b)-> b.getValue()-a.getValue()).map(Map.Entry::getKey).collect(Collectors.toList());
        Map<Character, Character> map=new HashMap<>();
        for(int i=0;i<Math.min(sorted.size(),COMMON_BIGRAMS.length);i++){
            String bi=sorted.get(i);
            String eng=COMMON_BIGRAMS[i];
            map.putIfAbsent(bi.charAt(0),eng.charAt(0));
            map.putIfAbsent(bi.charAt(1),eng.charAt(1));
        }
        return map;
    }

    private static Map<Character, Character> getTrigramMapping(String text){
        String compact=text.replaceAll(" ","");
        Map<String,Integer> freq=new HashMap<>();
        for(int i=0;i<compact.length()-2;i++){
            String tri=compact.substring(i,i+3);
            freq.put(tri,freq.getOrDefault(tri,0)+1);
        }
        List<String> sorted=freq.entrySet().stream().sorted((a,b)-> b.getValue()-a.getValue()).map(Map.Entry::getKey).collect(Collectors.toList());
        Map<Character, Character> map=new HashMap<>();
        for(int i=0;i<Math.min(sorted.size(),COMMON_TRIGRAMS.length);i++){
            String tri=sorted.get(i);
            String eng=COMMON_TRIGRAMS[i];
            for(int j=0;j<3;j++) map.putIfAbsent(tri.charAt(j),eng.charAt(j));
        }
        return map;
    }
    private static String applyMapping(String text,Map<Character,Character> map){
        StringBuilder sb=new StringBuilder();
        for(char c : text.toCharArray())
            sb.append(c==' '?' ' : map.getOrDefault(c,c));
        return sb.toString();
    }
    private static String readFile(String filename) throws IOException{
        StringBuilder sb = new StringBuilder();
        try(BufferedReader br = new BufferedReader(new FileReader(filename))){
            String line;
            while((line = br.readLine()) != null) sb.append(line).append("\n");
        }
        return sb.toString();
    }
}
