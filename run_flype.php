#!/usr/bin/php
<?php

$number_of_alternating_knots = [
        0, 0, 0, // 0, 1, and 2 crossings have no meaning
        1, 1, 2, 3, 7, 18, 41, 123, 367, 1288, 4878, 19536, 85263, 379799
];

exec("/home/dj/CLionProjects/c-flypes/link.sh");

for($cr_num = 3; $cr_num <= 16; $cr_num++) {
    for($ordering = 1; $ordering <= $number_of_alternating_knots[$cr_num]; $ordering++) {

        $output = shell_exec("/home/dj/CLionProjects/c-flypes/a.out $cr_num $ordering");
        echo $output;

        if($ordering % 100 == 0) {
            echo "Done with first $ordering of $cr_num crossing knots.\n";
        }

    }
    echo "Done with $cr_num crossings.\n\n";
}
