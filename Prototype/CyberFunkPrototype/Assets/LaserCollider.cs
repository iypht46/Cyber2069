using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LaserCollider : MonoBehaviour
{
    Laser laser;

    // Start is called before the first frame update
    void Start()
    {
        laser = GameObject.Find("Laser").GetComponent<Laser>();
    }

    void Update()
    {
        
    }

    void OnTriggerStay2D(Collider2D other)
    {

        if (other.tag == "Enemy")
        {
            other.GetComponent<HP>().hp -= laser.LaserDMG;
        }

    }
}
