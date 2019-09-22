using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Explosion : MonoBehaviour
{
    [SerializeField] float DamageRadius = 2;
    [SerializeField] float Damage = 1;

    SpawnSystem spawner;
    private void Awake()
    {
        spawner = GameObject.Find("Spawner").GetComponent<SpawnSystem>();
    }

    public void Explode()
    {
        Collider2D[] hits = Physics2D.OverlapCircleAll(this.transform.position, DamageRadius);
        foreach(Collider2D hit in hits)
        {
            if (hit.GetComponent<HP>())
            {
                hit.GetComponent<HP>().TakeDamage(Damage);
            }
        }

        //spawner effect
        GameObject explosion = spawner.GetObjectFromPool("Explosion");
        explosion.transform.position = this.transform.position;
        explosion.transform.localScale = Vector3.one * (DamageRadius / 5);
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(this.transform.position, DamageRadius);
    }
}
