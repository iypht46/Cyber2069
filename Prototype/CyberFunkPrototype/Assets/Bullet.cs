using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;

public class Bullet : MonoBehaviour
{
    [SerializeField] private UnityEvent ContactEvent;
    [SerializeField] private int Damage = 1;
    [SerializeField] private float lifeTime = 2;

    private void FixedUpdate()
    {
        Invoke("Deactive",lifeTime);
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Enemy") && !collision.GetComponent<HP>().isDead)
        {
            collision.GetComponent<HP>().TakeDamage(Damage);
            ContactEvent.Invoke();
        }
    }

    void Deactive()
    {
        CancelInvoke();
        this.gameObject.SetActive(false);
    }
}
