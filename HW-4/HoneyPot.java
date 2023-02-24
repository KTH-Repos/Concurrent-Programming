import java.util.ArrayList;

//This solution is not fair as only one bee always fills in portions
//in honeypot. The other bees are starved and dont get access to the monitor

public class HoneyPot {

    int honeyPotCapacity = 8;
    int honeyPortion = 0;

    synchronized void putHoney(Bees bees) {
        while (true) {
            honeyPortion++;
            if (honeyPortion == honeyPotCapacity) {
                try {
                    System.out.println("bee " + bees.getName() + " made honeypot full, about to wake up bear ...");
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } else {
                System.out.println("bee " + bees.getName() + " filled with one portion, currently " + honeyPortion
                        + " portions in honeypot");
                notify();
                try {
                    bees.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

            }
        }
    }

    synchronized void takeHoney(Bear bear) {
        while (true) {
            if (honeyPortion != honeyPotCapacity) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } else {
                honeyPortion = 0;
                System.out.println("the bear ate all honeyportions, 0 portions left");
                notify();
            }
        }
    }

    public static void main(String[] args) {
        HoneyPot pot = new HoneyPot();
        final int PRODUCERS = 3;
        ArrayList<Bees> bees = new ArrayList<>();
        Bear bear = new Bear(pot);
        for (int i = 0; i < PRODUCERS; i++) {
            bees.add(new Bees(Integer.toString(i), pot));
        }

        bear.start();
        for (int i = 0; i < bees.size(); i++) {
            bees.get(i).start();
        }

        try {
            bear.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        for (int i = 0; i < bees.size(); i++) {
            try {
                bees.get(i).join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }
}
