# Assignment 2 report - part 1

## 1. Draw bounding rectangles
In this section we've traveled through model triangles.\
We created each triangle bounding rectangle by calculating max and min x coordinates and max and min y coordinates.\
We set the color by z cooridnates value, as we defined deeper rectangles to be brighter, shallow rectangles will get warm colors.

![image](https://user-images.githubusercontent.com/100403800/210892800-113656e6-93ac-45f0-9b87-3bfcfd195c77.png)

## Fill triangle with random color  
we choosed a random color for each triangle and with edge walking approach we were able to execute the task
![edge walking color](https://user-images.githubusercontent.com/100276577/211213260-f7c8def0-9e25-422e-9bac-6d33e4937744.png)

## Fill triangle using z buffer algorithm
we implemented z buffer algorithm, and because of that you can see at the pictures below that we no longer have an overlapping triangle
![z buffer color](https://user-images.githubusercontent.com/100276577/211213405-ee6362a7-3e0b-4e3c-8891-f1c46653fb49.png)

## Color buffer and visualized Z buffer 

![Screen Shot 2023-01-10 at 20 10 13](https://user-images.githubusercontent.com/100276577/211629562-ea0e4bec-f41e-4907-8141-7cea23d2da94.png)
![Screen Shot 2023-01-10 at 20 10 18](https://user-images.githubusercontent.com/100276577/211629572-298fd769-9f1c-4d94-a573-81d3be37d0c6.png)
![Screen Shot 2023-01-10 at 20 07 46](https://user-images.githubusercontent.com/100276577/211629580-69db86eb-0969-4cbb-b230-bd3c432cf0a9.png)
![Screen Shot 2023-01-10 at 20 07 51](https://user-images.githubusercontent.com/100276577/211629592-992f98a8-784b-4219-8e95-76f966090184.png)
![Screen Shot 2023-01-10 at 20 05 54](https://user-images.githubusercontent.com/100276577/211629600-142f1a4e-07f4-4a88-8448-ead0f63fe903.png)
![Screen Shot 2023-01-10 at 20 05 59](https://user-images.githubusercontent.com/100276577/211629604-6b6d880a-9f50-4779-a0b9-45b2b20e2abe.png)
