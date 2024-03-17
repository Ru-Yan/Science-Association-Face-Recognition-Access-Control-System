using System;
using System.Linq;
using Facecore.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace Facecore.Controllers
{
    [Route("api/[controller]/[action]")]
    [ApiController]
    public class ActiController : Controller
    {
        private readonly ILogger<UserController> _logger;
        private readonly FacecoreContext _context;

        public ActiController(ILogger<UserController> logger, FacecoreContext context)
        {
            _logger = logger;
            _context = context;
        }

        public ActionResult Home()
        {
            try
            {
                var acti = _context.acti;
                ViewBag.actis = acti;
                return View();
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
        }

        public ActionResult addActi([FromForm]acti activity)
        {
            _context.acti.Add(activity);
            _context.SaveChanges();
            return RedirectToAction(nameof(Home));
        }

        public object addActiApi(acti activity)
        {
            try
            {
                _context.acti.Add(activity);
                _context.SaveChanges();
                return activity;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(502);
            }
        }

        public ActionResult deleteActi(int id)
        {
            var i = _context.acti.First(e => e.Id == id);
            _context.acti.Remove(i);
            _context.SaveChanges();
            return RedirectToAction(nameof(Home));
        }

        public object getAllActi()
        {
            try
            {
                var acti = _context.acti;
                return acti;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
            
        }

        public object getActiInfo(int id)
        {
            try
            {
                var acti = _context.acti.First(e => e.Id == id);
                return acti;
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return StatusCode(404);
            }
            
        }
    }
}
