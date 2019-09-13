using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class HP : MonoBehaviour
{
    public UnityEvent dead;
    public float Maxhp;
    public float hp;

    void OnEnable()
    {
        hp = Maxhp;
    }

    // Update is called once per frame
    void Update()
    {
        if(hp <= 0)
        {
            dead.Invoke();
        }
    }

}
