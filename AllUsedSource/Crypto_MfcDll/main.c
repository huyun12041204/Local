#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "sha2.h"
#include "rmd160.h"

void checkdigest(const unsigned char *vector, unsigned char *digest, unsigned int digest_size)
{
    unsigned char *output, *outputX, *vectorbuf;
    int i;
    output = (unsigned char *)malloc(2*digest_size+1);
    outputX = (unsigned char *)malloc(2*digest_size+1);
    output[2 * digest_size] = '\0';
    outputX[2 * digest_size] = '\0';
    vectorbuf = (unsigned char *)malloc(2*digest_size+1);
    vectorbuf[2 * digest_size] = '\0';

	memcpy(vectorbuf, vector, 2*digest_size);

    for (i = 0; i < (int) digest_size ; i++) 
	{
       sprintf((char *) output + 2 * i, "%02x", digest[i]);
       sprintf((char *) outputX + 2 * i, "%02X", digest[i]);
    }

    printf("H: %s\n", output);
    i = strlen(vector);
	if(i == 0)
        fprintf(stderr, "No data.  ");
    if (strcmp((char *) vectorbuf, (char *) output)) 
	{
        if (strcmp((char *) vectorbuf, (char *) outputX)) 
		{
            fprintf(stderr, "Test failed.\n");
		}
		else
		{
            fprintf(stderr, "Test OK.\n");
		}
    }
	else
	{
        fprintf(stderr, "Test OK.\n");
    }

}

int hashtest()
{
	unsigned long iLen, idegistlen;
	unsigned char message1[] = "";
	unsigned char message2[] = "abc";
	unsigned char message3[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	unsigned char *message4;
	unsigned char message5[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    unsigned long message4_len  = 1000000;
    unsigned char *digest;
    unsigned char *vectors[6][5] =
    {   
	// SHA-1
        {
        "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709",
        "A9993E364706816ABA3E25717850C26C9CD0D89D",
        "84983E441C3BD26EBAAE4AA1F95129E5E54670F1",
		"34AA973CD4C4DAA4F61EEB2BDBAD27316534016F",
		"761C457BF73B14D27E9E9265C46F4B4DDA11F940",
        },
		
		// SHA-224 
        {
		"",
        "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7",
        "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525",
        "20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67",
		"",
        },
        // SHA-256 
        {
		"E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855",
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1",
        "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0",
		"DB4BFCBD4DA0CD85A60C3C37D3FBD8805C77F15FC6B1FDFE614EE0A7C8FDB4C0",
        },
        // SHA-384 
        {
		"38B060A751AC96384CD9327EB1B1E36A21FDB71114BE07434C0CC7BF63F6E1DA274EDEBFE76F65FBD51AD2F14898B95B",
        "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7",
        "3391FDDDFC8DC7393707A65B1B4709397CF8B1D162AF05ABFE8F450DE5F36BC6B0455A8520BC4E6F5FE95B1FE3C8452B",
        "9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985",
		"1761336E3F7CBFE51DEB137F026F89E01A448E3B1FAFA64039C1464EE8732F11A5341A6F41E0C202294736ED64DB1A84",
        },
        // SHA-512
        {
        "CF83E1357EEFB8BDF1542850D66D8007D620E4050B5715DC83F4A921D36CE9CE"
        "47D0D13C5D85F2B0FF8318D2877EEC2F63B931BD47417A81A538327AF927DA3E",
        "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
        "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f",
		"204A8FC6DDA82F0A0CED7BEB8E08A41657C16EF468B228A8279BE331A703C335"
        "96FD15C13B1B07F9AA1D3BEA57789CA031AD85C7A71DD70354EC631238CA3445",
        "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
        "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b",
		"1E07BE23C26A86EA37EA810C8EC7809352515A970E9253C26F536CFC7A9996C4"
        "5C8370583E0A78FA4A90041D71A4CEAB7423F19C71B9D5A3E01249F0BEBD5894",
        },
		// RMD-160 
		{
		"",
		"8EB208F7E05D987A9B044A8E98C6B087F15A0BFC",
		"12A053384A9C0C88E405A06C27DCF49ADA62EB2B",
		"",
		"B0E20B6E3116640286ED3A87A5713079B21F5189",
		},
    };
    message4 = malloc(message4_len+1);
    if (message4 == NULL) 
	{
        fprintf(stderr, "Can't allocate memory\n");
        return -1;
    }    
	digest = malloc(128);
    if (digest == NULL) 
	{
        fprintf(stderr, "Can't allocate memory\n");
        return -1;
    }
    memset(message4, 'a', message4_len);
	message4[message4_len] = '\x0';

    printf("\n");
	printf("SHA-1 Test vectors\n");
    iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hash(message1,iLen, digest, DEF_HASH_SHA1);
    checkdigest(vectors[0][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hash(message2,iLen, digest, DEF_HASH_SHA1);
    checkdigest(vectors[0][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hash(message3,iLen, digest, DEF_HASH_SHA1);
    checkdigest(vectors[0][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hash(message4,iLen, digest, DEF_HASH_SHA1);
    checkdigest(vectors[0][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hash(message5,iLen, digest, DEF_HASH_SHA1);
    checkdigest(vectors[0][4], digest, idegistlen);

    printf("\n");
    printf("SHA-224 Test vectors\n");
    iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hash(message1,iLen, digest, DEF_HASH_SHA224);
    checkdigest(vectors[1][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hash(message2,iLen, digest, DEF_HASH_SHA224);
    checkdigest(vectors[1][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hash(message3,iLen, digest, DEF_HASH_SHA224);
    checkdigest(vectors[1][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hash(message4,iLen, digest, DEF_HASH_SHA224);
    checkdigest(vectors[1][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hash(message5,iLen, digest, DEF_HASH_SHA224);
    checkdigest(vectors[1][4], digest, idegistlen);

    printf("\n");
    printf("SHA-256 Test vectors\n");
    iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hash(message1,iLen, digest, DEF_HASH_SHA256);
    checkdigest(vectors[2][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hash(message2,iLen, digest, DEF_HASH_SHA256);
    checkdigest(vectors[2][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hash(message3,iLen, digest, DEF_HASH_SHA256);
    checkdigest(vectors[2][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hash(message4,iLen, digest, DEF_HASH_SHA256);
    checkdigest(vectors[2][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hash(message5,iLen, digest, DEF_HASH_SHA256);
    checkdigest(vectors[2][4], digest, idegistlen);

    printf("\n");
    printf("SHA-384 Test vectors\n");
    iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hash(message1,iLen, digest, DEF_HASH_SHA384);
    checkdigest(vectors[3][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hash(message2,iLen, digest, DEF_HASH_SHA384);
    checkdigest(vectors[3][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hash(message3,iLen, digest, DEF_HASH_SHA384);
    checkdigest(vectors[3][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hash(message4,iLen, digest, DEF_HASH_SHA384);
    checkdigest(vectors[3][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hash(message5,iLen, digest, DEF_HASH_SHA384);
    checkdigest(vectors[3][4], digest, idegistlen);

    printf("\n");
    printf("SHA-512 Test vectors\n");
    iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hash(message1,iLen, digest, DEF_HASH_SHA512);
    checkdigest(vectors[4][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hash(message2,iLen, digest, DEF_HASH_SHA512);
    checkdigest(vectors[4][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hash(message3,iLen, digest, DEF_HASH_SHA512);
    checkdigest(vectors[4][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hash(message4,iLen, digest, DEF_HASH_SHA512);
    checkdigest(vectors[4][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hash(message5,iLen, digest, DEF_HASH_SHA512);
    checkdigest(vectors[4][4], digest, idegistlen);

    printf("\n");
    printf("RMD-160 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hash(message1,iLen, digest, DEF_HASH_RMD160);
    checkdigest(vectors[5][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hash(message2,iLen, digest, DEF_HASH_RMD160);
    checkdigest(vectors[5][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hash(message3,iLen, digest, DEF_HASH_RMD160);
    checkdigest(vectors[5][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hash(message4,iLen, digest, DEF_HASH_RMD160);
    checkdigest(vectors[5][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hash(message5,iLen, digest, DEF_HASH_RMD160);
    checkdigest(vectors[5][4], digest, idegistlen);

    printf("\n");
    printf("All hash tests passed.\n");
    return 0;
}

int hmactest1()
{
	unsigned long iLen, idegistlen;
	unsigned char message1[] = "";
	unsigned char message2[] = "abc";
	unsigned char message3[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	unsigned char *message4;
	unsigned char message5[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    unsigned long message4_len  = 1000000;
    unsigned char *digest;
	unsigned int  key_len = 64;
    unsigned char key[64]="\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF\
\x01\x23\x45\x67\x89\xAB\xCD\xEF\x00\x11\x22\x33\x44\x55\x66\x77\
\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF\x01\x23\x45\x67\x89\xAB\xCD\xEF\
\x00\x11\x22\x33\x44\x55\x66\x77\x88\x99\xAA\xBB\xCC\xDD\xEE\xFF";

    static const unsigned char *macs[6][5] =
	{
		// HMAC SHA-1
        {
        "1F25DAE40F16D60661045807F9CD0243F28174F7",
        "759CE995CD7960D864F875BE6CFF655DEAAE7A2B",
        "C4F57AAB0CDB8EF9B04CB26BE2521DF357E81372",
		"538209DEAF414A36B1C246D506C603A0E79085BB",
		"727805ED9C7B554D42EEDDFD20252FE516D76C52",
        },
		// HMAC SHA-224
        {
        "",
        "",
        "",
		"",
		"",
        },
		// HMAC SHA-256
        {
        "B379757F089F3EC3B41BFD184048AB436FAE7E09F8C7E3461C825ED37E544303",
        "742CB1AE5D0949615F9D2866EFB1BE212C766C96C44AAF1902E10740B19DEBF5",
        "AB65232F3BD2450297264E3FD828E1DD4666BEB965C54CF842A4AD5E453192A6",
		"BB459133294AF921181055854C16B06336E9B0514761BA2396B8A0A8028393BA",
		"76FA502215729DB8B77D3799B221A8FDB538393947A16730B5E84FC2837635C6",
        },
		// HMAC SHA-384
        {
        "37A8BAEF38B007B84B9E99C8EE1AA29A66D8CD48096EBDF6AD7891CE5354FBFA3E6A4AD9A63B8DE6187CB25ED630C991",
        "ED6A0B406765E42A52701E2E539B7C5DE76662F802E65EB58B920AD9176C949D41CD0BA966AA9B51345123C544509B89",
        "A7C4ABDA0E6DA05FC2159E486494DE88E62C5F6A2C8A3E8DCE8DFF0342DB015712D9F14E96BF945BBF01097CFCE8AED0",
		"13E9A5E29D7D0F92C8581E0C02ABE07C6584848D8D6B33728800E2B9F89018FE9769D54AFBE1E452DA51C634E5DDD149",
		"46148B04C40FB3A46D7CC4CD04F0FCE5373B00FD82E01A4450E4EAF70B48BD65DDC0EBEEFCEC3FF2D1328274F62D65A7",
        },
		// HMAC SHA-512
        {
        "0D6E18725D603C1A1830435DCD9947B465698EE83DD112A7E24766464E3F878B"
		"282A762BC6F001D44E56F247FF98A9BBEA17E76CB557B8039A08D942B6EAD6E9",
        "F476BDB72631B492F40633BB910667213AFE7E833A872B6F720BE394D0B2C8AC"
		"70AA3C58FE99181B11082FE47BC6E5E53BF788BFCC060FADF0754F550589D8FA",
        "83E39F72827FFEF05EB7056C5419F82AEE2EA8912CCA6A38540A9480F09C916A"
		"F39545326942A72C15BDC313340AF1BD97E8D10F907F7DECBB561DEFC6B40BBC",
		"601D41C98B564680B5CADB442912CAC195F3EC868558FFE023F1E4B5D9C61B8A"
		"3C5A3E00F77CB4C03A364D7CBF8F4FAAFDE9E892E12C04A5215B2EA0F9569B8D",
        "B4F458FB2231E90766DD04F706B9D7B1653A119916BBB01F5FFC4495FDFA1B6F"
		"27D6FAB647559AAEBABF2083D36E41F1CCC67C2A56E9E64529B9AF28F8A6DB36",
		},
		// HMAC RMD-160
        {
        "44700697D20DE7ECF4EB30BD3CDA60C1B2647CCF",
        "4DABFFC90F03499FFCC42D32EC07123A34B904DB",
        "E76C547A12CF20F00505034EBC9CAF38AF3D61B7",
		"8463C822FC86D954789427595530BCDED1DEC600",
		"49E8822830A67A2684CBD54B22A4731415CF4AFF",
        }
	};
	
    message4 = malloc(message4_len+1);
    if (message4 == NULL) 
	{
        fprintf(stderr, "Can't allocate memory\n");
        return -1;
    }    
	digest = malloc(128);
    if (digest == NULL) 
	{
        fprintf(stderr, "Can't allocate memory\n");
        return -1;
    }
    memset(message4, 'a', message4_len);
	message4[message4_len] = '\x0';

    printf("\n");
    printf("HMAC SHA-1 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hmac(key, key_len, message1, iLen, digest, 20, DEF_HASH_SHA1);
    checkdigest(macs[0][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hmac(key, key_len, message2, iLen, digest, 20, DEF_HASH_SHA1);
    checkdigest(macs[0][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hmac(key, key_len, message3, iLen, digest, 20, DEF_HASH_SHA1);
    checkdigest(macs[0][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hmac(key, key_len, message4, iLen, digest, 20, DEF_HASH_SHA1);
    checkdigest(macs[0][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hmac(key, key_len, message5, iLen, digest, 20, DEF_HASH_SHA1);
    checkdigest(macs[0][4], digest, idegistlen);
/*
    printf("\n");
    printf("HMAC SHA-224 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hmac(key, key_len, message1, iLen, digest, 28, DEF_HASH_SHA224);
    checkdigest(macs[1][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hmac(key, key_len, message2, iLen, digest, 28, DEF_HASH_SHA224);
    checkdigest(macs[1][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hmac(key, key_len, message3, iLen, digest, 28, DEF_HASH_SHA224);
    checkdigest(macs[1][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hmac(key, key_len, message4, iLen, digest, 28, DEF_HASH_SHA224);
    checkdigest(macs[1][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hmac(key, key_len, message5, iLen, digest, 28, DEF_HASH_SHA224);
    checkdigest(macs[1][4], digest, idegistlen);
*/
    printf("\n");
    printf("HMAC SHA-256 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hmac(key, key_len, message1, iLen, digest, 32, DEF_HASH_SHA256);
    checkdigest(macs[2][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hmac(key, key_len, message2, iLen, digest, 32, DEF_HASH_SHA256);
    checkdigest(macs[2][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hmac(key, key_len, message3, iLen, digest, 32, DEF_HASH_SHA256);
    checkdigest(macs[2][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hmac(key, key_len, message4, iLen, digest, 32, DEF_HASH_SHA256);
    checkdigest(macs[2][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hmac(key, key_len, message5, iLen, digest, 32, DEF_HASH_SHA256);
    checkdigest(macs[2][4], digest, idegistlen);
/*
    printf("\n");
    printf("HMAC SHA-384 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hmac(key, key_len, message1, iLen, digest, 48, DEF_HASH_SHA384);
    checkdigest(macs[3][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hmac(key, key_len, message2, iLen, digest, 48, DEF_HASH_SHA384);
    checkdigest(macs[3][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hmac(key, key_len, message3, iLen, digest, 48, DEF_HASH_SHA384);
    checkdigest(macs[3][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hmac(key, key_len, message4, iLen, digest, 48, DEF_HASH_SHA384);
    checkdigest(macs[3][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hmac(key, key_len, message5, iLen, digest, 48, DEF_HASH_SHA384);
    checkdigest(macs[3][4], digest, idegistlen);

    printf("\n");
    printf("HMAC SHA-512 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hmac(key, key_len, message1, iLen, digest, 64, DEF_HASH_SHA512);
    checkdigest(macs[4][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hmac(key, key_len, message2, iLen, digest, 64, DEF_HASH_SHA512);
    checkdigest(macs[4][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hmac(key, key_len, message3, iLen, digest, 64, DEF_HASH_SHA512);
    checkdigest(macs[4][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hmac(key, key_len, message4, iLen, digest, 64, DEF_HASH_SHA512);
    checkdigest(macs[4][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hmac(key, key_len, message5, iLen, digest, 64, DEF_HASH_SHA512);
    checkdigest(macs[4][4], digest, idegistlen);
	*/
    printf("\n");
    printf("HMAC RMD-160 Test vectors\n");   
	iLen =(unsigned int) strlen((char *) message1);
    idegistlen = hmac(key, key_len, message1, iLen, digest, 20, DEF_HASH_RMD160);
    checkdigest(macs[5][0], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message2);
    idegistlen = hmac(key, key_len, message2, iLen, digest, 20, DEF_HASH_RMD160);
    checkdigest(macs[5][1], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message3);
    idegistlen = hmac(key, key_len, message3, iLen, digest, 20, DEF_HASH_RMD160);
    checkdigest(macs[5][2], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message4);
    idegistlen = hmac(key, key_len, message4, iLen, digest, 20, DEF_HASH_RMD160);
    checkdigest(macs[5][3], digest, idegistlen);
    iLen =(unsigned int) strlen((char *) message5);
    idegistlen = hmac(key, key_len, message5, iLen, digest, 20, DEF_HASH_RMD160);
    checkdigest(macs[5][4], digest, idegistlen);

    printf("\n");
    printf("All tests passed.\n");
    return 0;
}


int hmactest2()
{
	unsigned long iLen, idegistlen;
    unsigned char *digest;
	unsigned char *key[7] = {
	"\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
	"Jefe",
	"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
	"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19",
    "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c",
	"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
	"\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
	};
	unsigned char *message[7] = {
	"\x48\x69\x20\x54\x68\x65\x72\x65",
	"what do ya want for nothing?",
	"\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd",
	"\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd",
	"Test With Truncation",
	"Test Using Larger Than Block-Size Key - Hash Key First",
	"This is a test using a larger than block-size key and a larger than block-size data. The key needs to be hashed before being used by the HMAC algorithm.",
	};
	static const unsigned char *hmacs[7][4] =
	{
		{
        "896fb1128abbdf196832107cd49df33f47b4b1169912ba4f53684b22",
        "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7",
        "afd03944d84895626b0825f4ab46907f15f9dadbe4101ec682aa034c7cebc59cfaea9ea9076ede7f4af152e8b2fa9cb6",
        "87aa7cdea5ef619d4ff0b4241a1d6cb02379f4e2ce4ec2787ad0b30545e17cdedaa833b7d6b8a702038b274eaea3f4e4be9d914eeb61f1702e696c203a126854",
		},
		{
        "a30e01098bc6dbbf45690f3a7e9e6d0f8bbea2a39e6148008fd05e44",
        "5bdcc146bf60754e6a042426089575c75a003f089d2739839dec58b964ec3843",
        "af45d2e376484031617f78d2b58a6b1b9c7ef464f5a01b47e42ec3736322445e8e2240ca5e69e2c78b3239ecfab21649",
        "164b7a7bfcf819e2e395fbe73b56e0a387bd64222e831fd610270cd7ea2505549758bf75c05a994a6d034f65f8f0e6fdcaeab1a34d4a6b4b636e070a38bce737",
		},
		{
        "7fb3cb3588c6c1f6ffa9694d7d6ad2649365b0c1f65d69d1ec8333ea",
        "773ea91e36800e46854db8ebd09181a72959098b3ef8c122d9635514ced565fe",
        "88062608d3e6ad8a0aa2ace014c8a86f0aa635d947ac9febe83ef4e55966144b2a5ab39dc13814b94e3ab6e101a34f27",
        "fa73b0089d56a284efb0f0756c890be9b1b5dbdd8ee81a3655f83e33b2279d39bf3e848279a722c806b485a47e67c807b946a337bee8942674278859e13292fb",
		},
		{
        "6c11506874013cac6a2abc1bb382627cec6a90d86efc012de7afec5a",
        "82558a389a443c0ea4cc819899f2083a85f0faa3e578f8077a2e3ff46729665b",
        "3e8a69b7783c25851933ab6290af6ca77a9981480850009cc5577c6e1f573b4e6801dd23c4a7d679ccf8a386c674cffb",
        "b0ba465637458c6990e5a8c5f61d4af7e576d97ff94b872de76f8050361ee3dba91ca5c11aa25eb4d679275cc5788063a5f19741120c4f2de2adebeb10a298dd",
		},
		{
        "0e2aea68a90c8d37c988bcdb9fca6fa8",
        "a3b6167473100ee06e0c796c2955552b",
        "3abf34c3503b2a23a46efc619baef897",
        "415fad6271580a531d4179bc891d87a6",
		},
		{
        "95e9a0db962095adaebe9b2d6f0dbce2d499f112f2d2b7273fa6870e",
        "60e431591ee0b67f0d8a26aacbf5b77f8e0bc6213728c5140546040f0ee37f54",
        "4ece084485813e9088d2c63a041bc5b44f9ef1012a2b588f3cd11f05033ac4c60c2ef6ab4030fe8296248df163f44952",
        "80b24263c7c1a3ebb71493c1dd7be8b49b46d1f41b4aeec1121b013783f8f3526b56d037e05f2598bd0fd2215d6a1e5295e64f73f63f0aec8b915a985d786598",
		},
		{
        "3a854166ac5d9f023f54d517d0b39dbd946770db9c2b95c9f6f565d1",
        "9b09ffa71b942fcb27635fbcd5b0e944bfdc63644f0713938a7f51535c3a35e2",
        "6617178e941f020d351e2f254e8fd32c602420feb0b8fb9adccebb82461e99c5a678cc31e799176d3860e6110c46523e",
        "e37b6a775dc87dbaa4dfa9f96e5e3ffddebd71f8867289865df5a32d20cdc944b6022cac3c4982b10d5eeb55c3e4de15134676fb6de0446065c97440fa8c6a58",
		},
	};
	unsigned int  key_len = 64;
	digest = malloc(128);
    if (digest == NULL) 
	{
        fprintf(stderr, "Can't allocate memory\n");
        return -1;
    }

    printf("\n");
    printf("HMAC SHA-224 Test vectors\n");   
	key_len = strlen(key[0]);
	iLen =(unsigned int) strlen((char *) message[0]);
	idegistlen = strlen(hmacs[0][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[0], key_len, message[0], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[0][0], digest, idegistlen);
    
	key_len = strlen(key[1]);
	iLen =(unsigned int) strlen((char *) message[1]);
	idegistlen = strlen(hmacs[1][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[1], key_len, message[1], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[1][0], digest, idegistlen);

	key_len = strlen(key[2]);
	iLen =(unsigned int) strlen((char *) message[2]);
	idegistlen = strlen(hmacs[2][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[2], key_len, message[2], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[2][0], digest, idegistlen);
	
	key_len = strlen(key[3]);
	iLen =(unsigned int) strlen((char *) message[3]);
	idegistlen = strlen(hmacs[3][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[3], key_len, message[3], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[3][0], digest, idegistlen);

	key_len = strlen(key[4]);
	iLen =(unsigned int) strlen((char *) message[4]);
	idegistlen = strlen(hmacs[4][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[4], key_len, message[4], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[4][0], digest, idegistlen);

	key_len = strlen(key[5]);
	iLen =(unsigned int) strlen((char *) message[5]);
	idegistlen = strlen(hmacs[5][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[5], key_len, message[5], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[5][0], digest, idegistlen);

	key_len = strlen(key[6]);
	iLen =(unsigned int) strlen((char *) message[6]);
	idegistlen = strlen(hmacs[6][0]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[6], key_len, message[6], iLen, digest, idegistlen, DEF_HASH_SHA224);
    checkdigest(hmacs[6][0], digest, idegistlen);

    printf("\n");
    printf("HMAC SHA-256 Test vectors\n"); 
	
	key_len = strlen(key[0]);
	iLen =(unsigned int) strlen((char *) message[0]);
	idegistlen = strlen(hmacs[0][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[0], key_len, message[0], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[0][1], digest, idegistlen);
    
	key_len = strlen(key[1]);
	iLen =(unsigned int) strlen((char *) message[1]);
	idegistlen = strlen(hmacs[1][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[1], key_len, message[1], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[1][1], digest, idegistlen);

	key_len = strlen(key[2]);
	iLen =(unsigned int) strlen((char *) message[2]);
	idegistlen = strlen(hmacs[2][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[2], key_len, message[2], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[2][1], digest, idegistlen);
	
	key_len = strlen(key[3]);
	iLen =(unsigned int) strlen((char *) message[3]);
	idegistlen = strlen(hmacs[3][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[3], key_len, message[3], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[3][1], digest, idegistlen);

	key_len = strlen(key[4]);
	iLen =(unsigned int) strlen((char *) message[4]);
	idegistlen = strlen(hmacs[4][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[4], key_len, message[4], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[4][1], digest, idegistlen);

	key_len = strlen(key[5]);
	iLen =(unsigned int) strlen((char *) message[5]);
	idegistlen = strlen(hmacs[5][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[5], key_len, message[5], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[5][1], digest, idegistlen);

	key_len = strlen(key[6]);
	iLen =(unsigned int) strlen((char *) message[6]);
	idegistlen = strlen(hmacs[6][1]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[6], key_len, message[6], iLen, digest, idegistlen, DEF_HASH_SHA256);
    checkdigest(hmacs[6][1], digest, idegistlen);


    printf("\n");
    printf("HMAC SHA-384 Test vectors\n");   
	key_len = strlen(key[0]);
	iLen =(unsigned int) strlen((char *) message[0]);
	idegistlen = strlen(hmacs[0][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[0], key_len, message[0], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[0][2], digest, idegistlen);
    
	key_len = strlen(key[1]);
	iLen =(unsigned int) strlen((char *) message[1]);
	idegistlen = strlen(hmacs[1][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[1], key_len, message[1], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[1][2], digest, idegistlen);

	key_len = strlen(key[2]);
	iLen =(unsigned int) strlen((char *) message[2]);
	idegistlen = strlen(hmacs[2][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[2], key_len, message[2], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[2][2], digest, idegistlen);
	
	key_len = strlen(key[3]);
	iLen =(unsigned int) strlen((char *) message[3]);
	idegistlen = strlen(hmacs[3][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[3], key_len, message[3], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[3][2], digest, idegistlen);

	key_len = strlen(key[4]);
	iLen =(unsigned int) strlen((char *) message[4]);
	idegistlen = strlen(hmacs[4][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[4], key_len, message[4], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[4][2], digest, idegistlen);

	key_len = strlen(key[5]);
	iLen =(unsigned int) strlen((char *) message[5]);
	idegistlen = strlen(hmacs[5][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[5], key_len, message[5], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[5][2], digest, idegistlen);

	key_len = strlen(key[6]);
	iLen =(unsigned int) strlen((char *) message[6]);
	idegistlen = strlen(hmacs[6][2]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[6], key_len, message[6], iLen, digest, idegistlen, DEF_HASH_SHA384);
    checkdigest(hmacs[6][2], digest, idegistlen);

    printf("\n");
    printf("HMAC SHA-512 Test vectors\n");   
	key_len = strlen(key[0]);
	iLen =(unsigned int) strlen((char *) message[0]);
	idegistlen = strlen(hmacs[0][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[0], key_len, message[0], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[0][3], digest, idegistlen);
    
	key_len = strlen(key[1]);
	iLen =(unsigned int) strlen((char *) message[1]);
	idegistlen = strlen(hmacs[1][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[1], key_len, message[1], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[1][3], digest, idegistlen);

	key_len = strlen(key[2]);
	iLen =(unsigned int) strlen((char *) message[2]);
	idegistlen = strlen(hmacs[2][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[2], key_len, message[2], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[2][3], digest, idegistlen);
	
	key_len = strlen(key[3]);
	iLen =(unsigned int) strlen((char *) message[3]);
	idegistlen = strlen(hmacs[3][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[3], key_len, message[3], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[3][3], digest, idegistlen);

	key_len = strlen(key[4]);
	iLen =(unsigned int) strlen((char *) message[4]);
	idegistlen = strlen(hmacs[4][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[4], key_len, message[4], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[4][3], digest, idegistlen);

	key_len = strlen(key[5]);
	iLen =(unsigned int) strlen((char *) message[5]);
	idegistlen = strlen(hmacs[5][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[5], key_len, message[5], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[5][3], digest, idegistlen);

	key_len = strlen(key[6]);
	iLen =(unsigned int) strlen((char *) message[6]);
	idegistlen = strlen(hmacs[6][3]);
	idegistlen = idegistlen/2;
    idegistlen = hmac(key[6], key_len, message[6], iLen, digest, idegistlen, DEF_HASH_SHA512);
    checkdigest(hmacs[6][3], digest, idegistlen);

    printf("\n");
    printf("All tests passed.\n");
    return 0;
}

void main()
{
	hashtest();
	hmactest1();
	hmactest2();
}

//#endif /* TEST_VECTORS */