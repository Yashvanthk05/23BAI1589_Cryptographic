import java.util.*;

public class Decryptor {
    public static void main(String[] args) {
        String cipher = "tvbjbs,pcitjgkicpsptj,owipdbroe.gbsovrpiaoibj,ikepl.bpvjtpd.wwigipob.qidepskcp skeicpeviposbsgbsovpjetiwlpe.pci,.ktebseipdwisbpikgwjtvp bjejkgp jevpd.bbidepgbs,,sbhpvipdv.tipd.,,.kp .bctpsy.feptidfbjernpd.cjkgnpskcpcsespobjusdrpt.pevsepevipbitfwejkgpeiaep .fwcpyipbiswjtejdpriepistrpe.pskswrtihpsleibpljkjtvjkgpevipcbslepvipbi,.uicpdsojeswpwieeibtnpdvidmicpeviptoiwwjkgnpskcpiktfbicpevseptosditpskcpofkdefsej.kp ibipd.ktjteikepiuibr vibihpevjtpljkswpuibtj.kpk. pd.kesjktpiasdewrp.kipvfkcbicp .bctnp,smjkgpjepd.kuikjikepl.bpeitejkgptfytejefej.kpdjovibtpskcpiaow.bjkgpv. plbixfikdrpskswrtjtpbiuiswtposeeibktpjktjcipsoosbikewrpbskc.,pw..mjkgpikdbroeicp,ittsgitpcfbjkgpystjdpwsy.bse.brpterwipiaibdjtitpe.csrh";

        Map<Character, Integer> counts = new HashMap<>();
        for (char c : cipher.toCharArray()) {
            counts.put(c, counts.getOrDefault(c, 0) + 1);
        }

        List<Character> sortedChars = new ArrayList<>(counts.keySet());
        sortedChars.sort((a, b) -> counts.get(b).compareTo(counts.get(a)));

        String englishOrder = " etaoishnrdlumwfgypbvkjxqz";
        Map<Character, Character> map = new HashMap<>();

        for (int i = 0; i < sortedChars.size() && i < englishOrder.length(); i++) {
            map.put(sortedChars.get(i), englishOrder.charAt(i));
        }

        map.put('p', 'e');
        map.put('i', 'h');
        map.put('v', 't');
        map.put('j', 's');
        map.put('t', 'o');
        map.put('s', 'r');
        map.put('b', 'l');
        map.put('k', 'n');
        map.put('d', 'w');
        map.put('g', 'c');
        map.put('f', 'b');
        map.put('r', 'y');
        map.put('o', 'p');
        map.put('w', 'f');
        map.put('q', 'v');
        map.put('u', 'g');
        map.put('.', 'a');
        map.put(',', 'm');
        map.put('e', 'u');
        map.put(';', 'i');
        map.put('l', 'd');
        map.put(' ', ' ');

        StringBuilder decoded = new StringBuilder();
        for (char c : cipher.toCharArray()) {
            decoded.append(map.getOrDefault(c, c));
        }

        System.out.println(decoded.toString());
    }
}