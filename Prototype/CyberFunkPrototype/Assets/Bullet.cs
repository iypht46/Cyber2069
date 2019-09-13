using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class Bullet : MonoBehaviour
{
    [SerializeField] private UnityEvent ContactEvent;
    [SerializeField] private int Damage = 1;

    private void FixedUpdate()
    {
        Invoke("Deactive", 5);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Enemy"))
        {
            collision.GetComponent<HP>().hp -= Damage;
            ContactEvent.Invoke();
        }
    }

    void Deactive()
    {
        this.gameObject.SetActive(false);
    }
}
