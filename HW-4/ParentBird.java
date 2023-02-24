public class ParentBird extends Thread {
    WormDish dish;

    public ParentBird(WormDish dish) {
        this.dish = dish;
    }

    @Override
    public void run() {
        dish.putWorm(this);
    }

}
