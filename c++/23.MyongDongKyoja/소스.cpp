#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include <windows.h>

#include <process.h>

#include <tchar.h>



#define NUM_OF_CUSTOMER 20

#define RANGE_MIN 10

#define RANGE_MAX (30 - RANGE_MIN)

#define TABLE_CNT 10





HANDLE hSemaphore;

DWORD randTimeArr[50];



void TakeMeal(DWORD time)

{

    WaitForSingleObject(hSemaphore, INFINITE);

    _tprintf(_T("Enter Customer %d~ \n"), GetCurrentThreadId());



    _tprintf(_T("Customer %d having launch~ \n"), GetCurrentThreadId());

    Sleep(1000 * time);    // 식사중인 상태를 시뮬레이션 하는 함수



    ReleaseSemaphore(hSemaphore, 1, NULL);

    _tprintf(_T("Out Customer %d~ \n\n"), GetCurrentThreadId());

}





unsigned int WINAPI ThreadProc(LPVOID lpParam)

{

    TakeMeal((DWORD)lpParam);

    return 0;

}





int _tmain(int argc, TCHAR* argv[])

{

    DWORD dwThreadIDs[NUM_OF_CUSTOMER];

    HANDLE hThreads[NUM_OF_CUSTOMER];



    srand((unsigned)time(NULL));      // random function seed 설정





    // 쓰레드에게 전달 할 random 값. 총 50 개 생성

    for (int i = 0; i < NUM_OF_CUSTOMER; i++)

    {

        randTimeArr[i] = (DWORD)(

            ((double)rand() / (double)RAND_MAX) * RANGE_MAX + RANGE_MIN

            );

    }



    // 세마포어 생성

    hSemaphore = CreateSemaphore(

        NULL,    // 디폴트 보안 관리자

        TABLE_CNT,      // 세마포어 초기값

        TABLE_CNT,      // 세마포어 최댓값

        NULL     // unnamed 세마포어 구성

    );

    if (hSemaphore == NULL)

    {

        _tprintf(_T("CreateSemaphore error: %d\n"), GetLastError());

    }





    // Customer를 의미하는 쓰레드 생성

    for (int i = 0; i < NUM_OF_CUSTOMER; i++)

    {

        hThreads[i] = (HANDLE)

            _beginthreadex(

                NULL,

                0,

                ThreadProc,

                (void*)randTimeArr[i],

                CREATE_SUSPENDED,

                (unsigned*)&dwThreadIDs[i]

            );



        if (hThreads[i] == NULL)

        {

            _tprintf(_T("Thread creation fault! \n"));

            return -1;

        }

    }



    for (int i = 0; i < NUM_OF_CUSTOMER; i++)

    {

        ResumeThread(hThreads[i]);

    }



    WaitForMultipleObjects(NUM_OF_CUSTOMER, hThreads, TRUE, INFINITE);



    _tprintf(_T("----END-----------\n"));



    for (int i = 0; i < NUM_OF_CUSTOMER; i++)

    {

        CloseHandle(hThreads[i]);

    }



    CloseHandle(hSemaphore);



    return 0;

}
