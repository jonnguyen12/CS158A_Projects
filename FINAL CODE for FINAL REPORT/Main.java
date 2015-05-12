

import javax.swing.*;
import java.awt.*;
import java.io.Console;
import java.util.Scanner;

public class Main extends JPanel {

    public static Double result;
    public static final int SLOT_LIMIT = 20000; // Run until this many time slots have passed
    public static int AVG_PACKET_SIZE = 512; // Average packet size
    public static final double CONVERSION = 0.015625; // The conversion factor to go from bytes -> time slots
    public static int NUM_OF_STATIONS = 20;
    public static double MICROSECONDS = 51.2;
    public static double lambda = 20.0;
    public static int TIMES_TO_RUN = 1; // The times to run. If more than one, it will average the values
    public static double sum;
    public static Simulator mario;

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);
        System.out.println("Enter the number of stations: ");
        NUM_OF_STATIONS = scanner.nextInt();

        System.out.println("Enter the packet size: ");
        AVG_PACKET_SIZE = scanner.nextInt();

        System.out.println("Enter lambda: ");
        lambda = scanner.nextInt();

        System.out.println("How many times would you like to run this?: ");
        TIMES_TO_RUN = scanner.nextInt();


        double throughAverage = 0; // Used for calculating average throughput
        for (int i = 1; i <= TIMES_TO_RUN; i++) { // Start at index i=1 to avoid an off-by-one error
            mario = new Simulator(NUM_OF_STATIONS, lambda);
            sum = 0; // The summation of sums!
            while ((mario.getPacketsSent() * AVG_PACKET_SIZE * CONVERSION) <= SLOT_LIMIT) {
                result = mario.send(); // Do an initial send. If it fails, tryAgain(), else, hooray!
                if (result == -1) {
                    System.out.println("Packet sent at time: " + mario.tryAgain()); // Method tryAgain() internally loops
                } else {
                    System.out.println("Packet sent at time: " + result);
                }

                sum = +mario.getSentTime(); // Add the sent times together
                System.out.println("Packets sent successfully at time " + mario.getSuccessfulPacketsSent());
            }
            double throughput = (mario.getSuccessfulPacketsSent() * AVG_PACKET_SIZE * 8) / (SLOT_LIMIT * 51.2 * Math.pow(10, -6));
            // Print out the calculated throughput
            System.out.println("Calculated throughput = " + throughput); //Throughput!

            throughAverage += (double) (mario.getSuccessfulPacketsSent() * 8.0 * AVG_PACKET_SIZE) / (SLOT_LIMIT * 51.2 * Math.pow(10, -6));
        }

        System.out.println("\n--------------------\n");
        System.out.println("Number of stations = " + NUM_OF_STATIONS);
        System.out.println("Packet size = " + AVG_PACKET_SIZE);
        System.out.println("Lambda = " + lambda);
        System.out.println("Average throughput = " + (throughAverage / TIMES_TO_RUN));
        System.out.println("Traffic load = " + (NUM_OF_STATIONS * AVG_PACKET_SIZE * 8) / (lambda * MICROSECONDS) + "\n");
        System.out.println("Average delay is " + sum / mario.getSuccessfulPacketsSent());



    }
}