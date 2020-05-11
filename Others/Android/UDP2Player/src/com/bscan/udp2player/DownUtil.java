package com.bscan.udp2player;

//import entity.Header;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.RandomAccessFile;
import java.math.BigDecimal;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CountDownLatch;

import android.os.Environment;
import android.util.Log;

/**
 * @author xbl
 * @date 2018-09-17
 */
public class DownUtil {
	public static HttpURLConnection conn = null;
    //��������·��
    private String path;
    //ָ�������ص��ļ��ı���λ��
    private String sFileName;
    //���������̵߳�����
    private int threadNum;
    //���������̵߳Ķ���
    public static DownloadThread[] threads;
    //�����ļ����ܴ�С
    private long fileSize;
    //�̼߳�����
    private CountDownLatch latch;

    public DownUtil(String path,String targetFile,int threadNum){
        this.path = path;
        this.sFileName = targetFile;
        this.threadNum = threadNum;
        threads = new DownloadThread[threadNum];
        latch = new CountDownLatch(threadNum);
    }
    public byte[] downLoad() throws Exception{
        long t1 = System.currentTimeMillis();
        URL url = new URL(path);
        
        fileSize = -1;
        while(fileSize == -1)
        {
        	if(conn == null) {
		        conn = (HttpURLConnection)url.openConnection();
		        conn.setConnectTimeout(2 * 1000);
		        conn.setReadTimeout(9 * 1000);
		        //�������󷽷�
		        conn.setRequestMethod("GET");
		        Map<String, List<String>> aaa;
		        aaa=conn.getRequestProperties();
		        //������������
		        for (Map.Entry<String ,String> entry : StaticBufs.header.entrySet()) {
					Log.i("TAG", "Key = " + entry.getKey() + ", Value = " + entry.getValue());
					conn.setRequestProperty(entry.getKey(), entry.getValue());
				}
		        //Header.header.forEach((key, value) -> conn.setRequestProperty(key,value));
		
		        conn.setRequestProperty("Accept-Encoding", "identity");
        	}
        	else
        	{
        		conn.getOutputStream().write(("GET " + "/123.ts"+ " HTTP/1.1\r\n").getBytes());
        	}
	        //�õ��ļ���С
	        fileSize = conn.getContentLength();
	        
	        //fileSize = 15000;
	        //conn.disconnect();
        }
        long currentPartSize = fileSize / threadNum + 1;
        byte[]b = null;
        
        for(int i = 0;i < threadNum;i++){
            //����ÿ���̵߳�����λ��
            long startPos = i * currentPartSize;
            
            if(fileSize - startPos < currentPartSize)
            	currentPartSize = fileSize - startPos;
            if(sFileName.length() > 0){
	            //ÿ���߳�ʹ��һ��RandomAccessFile��������
	            RandomAccessFile currentPart = new RandomAccessFile(sFileName,"rw");
	            //��λ���̵߳�����λ��
	            currentPart.seek(startPos);
	            //���������߳�
	            threads[i] = new DownloadThread((int)startPos, (int)currentPartSize, currentPart , path , latch);
            }
            else{
            	if(b == null)
            		b = new byte[(int)fileSize];
            	//ByteArrayOutputStream bArray = new ByteArrayOutputStream(currentPartSize);
	            threads[i] = new DownloadThread((int)startPos, (int)currentPartSize, b , path , latch);
            }
            new Thread(threads[i]).start();
        }
        BigDecimal n = BigDecimal.ZERO;
        while (true){
            BigDecimal length = BigDecimal.ZERO;
            if (latch.getCount()==0 && BigDecimal.ONE.compareTo(n)<=0){break;}
            for (DownloadThread downloadThread:threads) {
                length = length.add(new BigDecimal(downloadThread.getLength()));
                Log.i("TAG", "������ "+downloadThread+"��" + downloadThread.getLength()+".ʣ." + downloadThread.getRemainLen());
            }
            n = length.divide(new BigDecimal(fileSize), 4, BigDecimal.ROUND_HALF_UP);
            Log.i("TAG", "������:" + length + "/" + fileSize + "/" + n.multiply(new BigDecimal("100"))+"%");
            Thread.sleep(1000);
        }
        long t2 = System.currentTimeMillis();
        Log.i("TAG", "������ɣ��ļ���СΪ"
        +String.format("%.3f", fileSize*1.0/(1024*1024))+"M["+ fileSize+ "]������ʱ"+(t2-t1)*1.0/1000+"��,ƽ��"
        +String.format("%.3f", fileSize*1.0/(1024*1024)/((t2-t1)/1000))+"MB/s");
		return b;
        
        
        //�̵߳ȴ�,һ���ļ����سɹ��������һ��
//        latch.await();
    }
	public static boolean isDowning(String sOneLine) {

        for (DownloadThread downloadThread:DownUtil.threads) {
        	if(downloadThread != null && downloadThread.getPath().equals(sOneLine))
        		return true;
        }
		return false;
	}
}