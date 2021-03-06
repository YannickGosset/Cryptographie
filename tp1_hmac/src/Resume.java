// -*- coding: utf-8 -*-

import java.io.*;
import java.security.*;

public class Resume
{
    public static void main(String[] args)
    {
        try {
            File fichier = new File("butokuden.jpg");
            FileInputStream fis = new FileInputStream(fichier);

            MessageDigest fonctionDeHachage = MessageDigest.getInstance("MD5");
            
            byte[] buffer = new byte[1024];
            int nbOctetsLus = fis.read(buffer);                   // Lecture du premier morceau
            while (nbOctetsLus != -1) {
                fonctionDeHachage.update(buffer, 0, nbOctetsLus); // Digestion du morceau
                nbOctetsLus = fis.read(buffer);                   // Lecture du morceau suivant
            }
            fis.close();

            byte[] resumeMD5 = fonctionDeHachage.digest();
            System.out.print("Le résumé MD5 du fichier \"butokuden.jpg\" vaut: 0x");
            for(byte k: resumeMD5)
                System.out.print(String.format("%02x", k));
            System.out.println();
        } catch (Exception e) { e.printStackTrace(); }
    }
}

/* 
$
$ cat butokuden.jpg | md5
aeef572459c1bec5f94b8d62d5d134b5
$ javac Resume.java
$ java Resume
Le résumé MD5 du fichier "butokuden.jpg" vaut: 0xaeef572459c1bec5f94b8d62d5d134b5
$
*/

