public class BabyBird extends Thread {
    WormDish dish;

    public BabyBird(String name, WormDish dish) {
        super(name);
        this.dish = dish;
    }

    @Override
    public void run() {
        dish.takeWorm(this);
    }

}