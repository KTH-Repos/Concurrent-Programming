import java.util.ArrayList;

//This solution is not fair as only one babybird always takes 
//worms from the dish. The other babybirds are starved and dont
//get access to the monitor.  

public class WormDish {

    int maxWorms = 5;
    int availableWorms = maxWorms;

    synchronized void putWorm(ParentBird bird) {
        while (true) {
            if (availableWorms != 0) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } else {
                availableWorms = maxWorms;
                System.out.println("parent just filled in the dish with " + availableWorms + " worms");
                notify();
            }
        }
    }

    synchronized void takeWorm(BabyBird bird) {
        while (true) {
            if (availableWorms == 0) {
                System.out.println("babybird " + bird.getName() + " found empty dish, waiting for parent...");
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            } else {
                availableWorms--;
                System.out.println(
                        "babybird " + bird.getName() + " ate one worm and there are " + availableWorms + " left");
                notify();
                try {
                    bird.sleep(2000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static void main(String[] args) {
        WormDish dish = new WormDish();
        final int CONSUMERS = 6;
        ArrayList<BabyBird> birds = new ArrayList<>();
        ParentBird parent = new ParentBird(dish);
        for (int i = 0; i < CONSUMERS; i++) {
            birds.add(new BabyBird(Integer.toString(i), dish));
        }

        parent.start();
        for (int i = 0; i < birds.size(); i++) {
            birds.get(i).start();
        }

        try {
            parent.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        for (int i = 0; i < birds.size(); i++) {
            try {
                birds.get(i).join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

}
